#include "GameEngine.h"
#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace GE {
	GameEngine::GameEngine() {
		w = 1024;
		h = 768;
		windowflags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
		FOV = 60.0f;
	}
	GameEngine::GameEngine(int _w, int _h, float _FOV) {
		w = _w;
		h = _h;
		windowflags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
		FOV = _FOV;
	}
	GameEngine::~GameEngine() {

	}

	bool GameEngine::init() {
		// Initialise SDL video subsystem
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Unable to initialise SDL! SDL error: " << SDL_GetError() << std::endl;

			return false;
		}

		// Set the OpenGL version for the program
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		// Set the type of profile which is core meaning modern OpenGL
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		// Create the window and frame features
		// (this has a fixed size and prepares window for OpenGL to render into)
		window = SDL_CreateWindow("SDL OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		// Check window was created
		if (window == nullptr) {
			std::cerr << "Unable to create window! SDL error: " << SDL_GetError() << std::endl;

			return false;
		}

		// Creates OpenGL context and links it to the window object
		// Context represents OpenGL for program such as objects and drawing
		glContext = SDL_GL_CreateContext(window);

		// Check context object was created
		if (glContext == nullptr) {
			std::cerr << "SDL could not create GL context! SDL error: " << SDL_GetError() << std::endl;

			return false;
		}

		// Initialise GLEW
		// GLEW binds the latest extensions of OpenGL to windows
		GLenum status = glewInit();

		// Check GLEW initialised ok
		if (status != GLEW_OK) {
			std::cerr << "Error initialising GLEW! Error: " << glewGetErrorString(status) << std::endl;

			return false;
		}

		// Turn on VSync
		if (vsync) {
			if (SDL_GL_SetSwapInterval(1) != 0) {
				std::cerr << "Warning: unable to set VSync! Error: " << SDL_GetError() << std::endl;

				return false;
			}
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Look down Z axis into the screen
		dist = glm::vec3(0.0f, 0.0f, -100.0f);

		// Create camera object
		cam = new Camera(glm::vec3(0.0f, 8.0f, 15.0f),	// Position
			glm::vec3(0.0f, 0.0f, 20.0f) + dist,	// Look at
			glm::vec3(0.0f, 1.0f, 0.0f),	// Up direction
			FOV, w / h, 0.1f, 150.0f);	// FOV, aspect ratio based on window dimensions, near and far clip planes

		// Create the TriangleRenderer object for the terrain
		triangle = new TriangleRenderer();

		// Initialise the terrain (two triangles)
		triangle->init();
		
		triangle->setPos(0.0f, 0.0f, 0.0f);
		triangle->setRotation(0.0f, 45.0f, 0.0f);
		triangle->setScale(100.0f, 100.0f, 100.0f);

		// Create the texture
		tex = new Texture("./textures/ForestTrail_textures.png");
		// Create a blank texture
		blank = new Texture("./textures/blank_texture.png");

		// Create the entities
		rock = new Entity("./models/rock.obj", tex);
		sign = new Entity("./models/sign.obj", tex);
		crate = new Entity("./models/crate.obj", tex);
		fence = new Entity("./models/fence.obj", tex);
		tree = new Entity("./models/tree.obj", tex);
		podium = new Entity("./models/podium.obj", tex);
		orb = new Entity("./models/orb.obj", tex);
		hedgehog = new Entity("./models/hedgehog.obj", tex);

		player = new Entity("./models/orb.obj", blank);

		rock->setName("Rock");
		sign->setName("Sign");
		crate->setName("Crate");
		fence->setName("Fence");
		tree->setName("Tree");
		podium->setName("POI - Podium");
		orb->setName("POI - Orb");
		hedgehog->setName("Hedgehog");

		// Put all of the entities in the vector
		loadedEntities.push_back(rock);
		loadedEntities.push_back(sign);
		loadedEntities.push_back(crate);
		loadedEntities.push_back(fence);
		loadedEntities.push_back(tree);
		loadedEntities.push_back(podium);
		loadedEntities.push_back(orb);
		loadedEntities.push_back(hedgehog);

		// Create the Model renderer object
		mr = new ModelRenderer();
		mr->init();

		rock->getTransform().setPosition(12.0f, 0.0f, 8.0f);
		sign->getTransform().setPosition(-2.0f, -1.0f, 3.0f);
		crate->getTransform().setPosition(-14.0f, 0.0f, -4.0f);
		fence->getTransform().setPosition(10.0f, 0.0f, -10.0f);
		fence->getTransform().setScale(0.8f, 0.8f, 0.8f);
		tree->getTransform().setPosition(9.0f, 0.0f, -12.0f);
		podium->getTransform().setPosition(-8.0f, 0.0f, 7.0f);
		orb->getTransform().setPosition(-8.0f, 0.0f, 7.0f);
		hedgehog->getTransform().setPosition(20.0f, 0.0f, 20.0f);

		player->getTransform().setPosition(0.0f, 2.0f, 15.0f);

		std::string skyboxPath = "./textures/skybox_textures/skybox";
		skybox = new SkyboxRenderer(skyboxPath + "_front.png", skyboxPath + "_back.png", skyboxPath + "_right.png", skyboxPath + "_left.png", skyboxPath + "_top.png", skyboxPath + "_bottom.png");

		// Create the camera controllers
		fpsCam = new FPSCameraController(cam);
		thirdCam = new ThirdPersonController(cam, player);

		fontFT = new FontRendererFT();
		fontFT->init();

		treeIr = new InstanceRenderer();
		treeIr->init();
		treeIr->setTexture(tex);

		rockIr = new InstanceRenderer();
		rockIr->init();
		rockIr->setTexture(tex);

		// Seed the random number generator
		srand(time(0));

		// However many trees are required
		const int num_trees = 30;
		// For loop to loop through all required trees
		for (int count = 0; count < num_trees; count++) {
			InstancePosRotScale treeInst = treeIr->setRandomPos(-60.0f, -20.0f, 0.0f, 0.0f, -60.0f, 60.0f, 20.0f, 60.0f, 0.8f, 1.3f);
			treeInstances.push_back(treeInst);
		}

		treeIr->setInstanceData(treeInstances);

		const int num_rocks = 20;
		for (int count = 0; count < num_rocks; count++) {
			InstancePosRotScale rockInst = rockIr->setRandomPos(-60.0f, 0.0f, 0.0f, 0.0f, -60.0f, 60.0f, 0.0f, 60.0f, 0.3f, 1.5f);
			rockInstances.push_back(rockInst);
		}

		rockIr->setInstanceData(rockInstances);

		// --[Constant stats values]--
		// Getting the number of entities and instanced objects
		numOfEntities = loadedEntities.size();
		numOfTrees = num_trees;
		numOfRocks = num_rocks;
		 
		// Sum vertices for entities
		for (auto& e : loadedEntities) {
			totalVertices += e->getModel()->getNumVertices();
		}
		// Include instanced models in the vertices count
		totalVertices += tree->getModel()->getNumVertices() * num_trees;
		totalVertices += rock->getModel()->getNumVertices() * num_rocks;
		// Calculate triangles
		totalTriangles = totalVertices / 3;

		// Initialise values for deltaTime & FPS counter
		lastTicks = SDL_GetTicks();
		
		fpsLastTime = SDL_GetTicks();
		frameCount = 0;
		currentFPS = 0;

		return true;
	}

	// A game would call this every frame to check if the user closed the window
	bool GameEngine::keep_running() {
		// Need to do this to keep the event queue up to date
		SDL_PumpEvents();

		SDL_Event evt;

		// Check for quit event
		if (SDL_PeepEvents(&evt, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
			// If user quit program, return false and stop it running
			return false;
		}

		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				switch (evt.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W:
					keyStates[UP] = true;
					break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S:
					keyStates[DOWN] = true;
					break;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A:
					keyStates[LEFT] = true;
					break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D:
					keyStates[RIGHT] = true;
					break;
				case SDL_SCANCODE_TAB:
					thirdPerson = !thirdPerson;
					break;
				case SDL_SCANCODE_F1:
					showStats = !showStats;
					break;
				}
			}
			if (evt.type == SDL_KEYUP) {
				switch (evt.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_W:
					keyStates[UP] = false;
					break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_S:
					keyStates[DOWN] = false;
					break;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_A:
					keyStates[LEFT] = false;
					break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_D:
					keyStates[RIGHT] = false;
					break;
				}
			}
		}

		return true;
	}

	// Update method which updates the game logic
	void GameEngine::update() {
		// Calculations for the deltaTime to run things consistently
		Uint32 now = SDL_GetTicks();
		float deltaTime = (now - lastTicks) / 1000.0f;	// Seconds

		lastTicks = now;

		// Calculations for the frames per second counter
		frameCount++;

		if (now - fpsLastTime > 1000) {
			currentFPS = frameCount;
			frameCount = 0;
			fpsLastTime = now;

			fpsText = "FPS: " + std::to_string(currentFPS);
		}

		// Changing between camera views
		if (thirdPerson) {
			thirdCam->update(deltaTime, keyStates);
		}
		else {
			fpsCam->update(deltaTime, keyStates);
		}

		// Moving the dynamic model
		glm::vec3 orbPos = orb->getTransform().getPosition();
		glm::vec3 orbRot = orb->getTransform().getRotation();
		float y = orbPos.y;

		if (y >= 2.0f) {
			y = 2.0f;
			dynamDirection = -1.0f;
		}
		else if (y <= -1.0f) {
			y = -1.0f;
			dynamDirection = 1.0f;
		}
		y += dynamSpeed * dynamDirection * deltaTime;
		// Applying the adjusted Y value to the models position
		orb->getTransform().setPosition(orbPos.x, y, orbPos.z);
		orb->getTransform().setRotation(orbRot.x, orbRot.y + 2.0f, orbRot.z);

		// Hedgehog movement
		glm::vec3 hedgehogPos = hedgehog->getTransform().getPosition();
		glm::vec3 hedgehogRot = hedgehog->getTransform().getRotation();

		if (hedgehogPos.x < 25) {
			hedgehog->getTransform().setPosition(hedgehogPos.x + 0.02f, hedgehogPos.y, hedgehogPos.z);
		}
		hedgehog->getTransform().setRotation(hedgehogRot.x, hedgehogRot.y + 0.5f, hedgehogRot.z);
	}

	// Draw method that renders the scene
	void GameEngine::draw() {
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// --[BACKGROUND OBJECTS]--
		// Render skybox first because it should be drawn behind everything
		skybox->draw(cam);

		// --[NORMAL OBJECTS]--
		// Draw instances of the tree model
		treeIr->drawInstanced(cam, tree->getModel());
		rockIr->drawInstanced(cam, rock->getModel());

		// Render the terrain (two triangles making up a quad)
		triangle->draw(cam);

		// Draw each entity in the vector of loaded entities
		for (int i = 0; i < loadedEntities.size(); i++) {
			mr->draw(cam, loadedEntities[i]);
		}

		// Switching between the different camera modes
		if (thirdPerson) {
			mr->draw(cam, player);
			fontFT->RenderText("[ THIRD-PERSON CAMERA ]", 48, h - 48, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
		else {
			fontFT->RenderText("[ FREE-ROAM CAMERA ]", 48, h - 48, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
		
		// --[UI ELEMENTS]--
		// Show game statistics
		if (showStats) {
			// --[DEBUGGING LIST]--
			int lineHeight = 20;
			int startX = w - 144;
			int startY = h - 24;

			fontFT->RenderText("---DEBUG STATS---", startX, startY, 0.35f, glm::vec3(1.0f));
			fontFT->RenderText(fpsText, startX, startY - lineHeight, 0.35f, glm::vec3(1.0f));
			fontFT->RenderText("Entities: " + std::to_string(numOfEntities), startX, startY - lineHeight * 2, 0.35f, glm::vec3(1.0f));
			fontFT->RenderText("Trees: " + std::to_string(numOfTrees), startX, startY - lineHeight * 3, 0.35f, glm::vec3(1.0f));
			fontFT->RenderText("Rocks: " + std::to_string(numOfRocks), startX, startY - lineHeight * 4, 0.35f, glm::vec3(1.0f));
			fontFT->RenderText("Vertices: " + std::to_string(totalVertices), startX, startY - lineHeight * 5, 0.35f, glm::vec3(1.0f));
			fontFT->RenderText("Triangles: " + std::to_string(totalTriangles), startX, startY - lineHeight * 6, 0.35f, glm::vec3(1.0f));


			// --[STATES LIST]--
			// State, Nearest, Position (Bottom left)
			int bottomX = 24;
			int bottomY = 120;

			// Inline if statement to compact code
			fontFT->RenderText((thirdPerson) ? "\nState: Grounded" : "\nState: Airborne", bottomX, bottomY, 0.35f, glm::vec3(1.0f));

			// Update nearest entity/object every 300ms
			static Uint32 lastNearestUpdate = 0;	// Static variable meaning it won't get reset every frame
			static float closestDist = INFINITY;
			Entity* closest = nullptr;
			static std::string closestName = "None";

			const Uint32 refreshInterval = 300;

			Uint32 now = SDL_GetTicks();
			if (now - lastNearestUpdate > refreshInterval) {
				lastNearestUpdate = now;

				closestDist = INFINITY;
				closest = nullptr;
				closestName = "None";

				// Account for current camera mode
				glm::vec3 viewCameraPos = !thirdPerson ? cam->getPos() : player->getTransform().getPosition();

				// Check for nearest entity
				for (auto& e : loadedEntities) {
					float dist = glm::length2(viewCameraPos - e->getTransform().getPosition());

					if (dist < closestDist) {
						closestDist = dist;
						closest = e;
						closestName = e->getName();
					}
				}
				// Check for nearest tree instance
				for (auto& inst : treeInstances) {
					glm::vec3 instPos(inst.posX, inst.posY, inst.posZ);
					float dist = glm::length2(viewCameraPos - instPos);

					if (dist < closestDist) {
						closestDist = dist;
						closestName = "Tree";
						closest = nullptr;
					}
				}
				// Check for nearest rock instance
				for (auto& inst : rockInstances) {
					glm::vec3 instPos(inst.posX, inst.posY, inst.posZ);
					float dist = glm::length2(viewCameraPos - instPos);

					if (dist < closestDist) {
						closestDist = dist;
						closestName = "Rock";
						closest = nullptr;
					}
				}

				closestDist = round(sqrt(closestDist) * 100) / 100;
			}
			// Display the closest entity/object
			if (closestDist < 15.0f) {
				fontFT->RenderText("Nearby: " + closestName + " (" + std::to_string(closestDist) + ")", bottomX, bottomY - lineHeight, 0.35f, glm::vec3(1.0f));
			}
			else {
				fontFT->RenderText("Nearby: None", bottomX, bottomY - lineHeight, 0.35f, glm::vec3(1.0f));
			}

			std::string posText = "Position: (0, 0, 0)";
			std::string rotText = "Pitch: 0, Yaw: 0";

			glm::vec3 pos = thirdPerson ? player->getTransform().getPosition() : cam->getPos();

			std::stringstream ss;
			ss << std::fixed << std::setprecision(2);
			ss << "Current Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")";
			posText = ss.str();

			float pitch = cam->getPitch();
			float yaw = thirdPerson ? player->getTransform().getRotation().y : cam->getYaw();

			ss.str("");
			ss.clear();
			ss << "Pitch: " << pitch << ", Yaw: " << yaw;
			rotText = ss.str();

			fontFT->RenderText(posText, bottomX, bottomY - lineHeight * 2, 0.35f, glm::vec3(1.0f));
			fontFT->RenderText(rotText, bottomX, bottomY - lineHeight * 3, 0.35f, glm::vec3(1.0f));
		}
		else {
			// Rendering the FPS text
			fontFT->RenderText(fpsText, w - 96, h - 48, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
		}

		SDL_GL_SwapWindow(window);
	}

	// Shutdown method closes OpenGL and SDL as well as destroying objects
	void GameEngine::shutdown() {
		// Get the triangle object to release it's resources
		if (triangle != nullptr) {
			triangle->destroy();
			delete triangle;
		}

		// Delete each entity from the vector of models
		for (int i = 0; i < loadedEntities.size(); i++) {
			delete loadedEntities[i];
		}

		if (player != nullptr) {
			delete player;
		}

		// Delete the model renderer
		if (mr != nullptr) {
			mr->destroy();
			delete mr;
		}

		if (fontFT != nullptr) {
			delete fontFT;
		}

		if (treeIr != nullptr) {
			treeIr->destroy();
			delete treeIr;
		}
		
		if (rockIr != nullptr) {
			rockIr->destroy();
			delete rockIr;
		}

		skybox->destroy();

		SDL_DestroyWindow(window);

		window = nullptr;

		SDL_Quit();
	}

	// Helper function to set the text in window title bar
	void GameEngine::setWindowTitle(const char* new_title) {
		SDL_SetWindowTitle(window, new_title);
	}

	// Helper function to display program information
	void display_info_message(const char* msg) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Basic Game Engine", msg, nullptr);
	}
}
