#include "GameEngine.h"
#include <iostream>

namespace GE {
	GameEngine::GameEngine() {
		w = 1080;
		h = 720;
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
		cam = new Camera(glm::vec3(0.0f, 10.0f, 5.0f),	// Position
			glm::vec3(0.0f, 0.0f, 20.0f) + dist,	// Look at
			glm::vec3(0.0f, 1.0f, 0.0f),	// Up direction
			FOV, w / h, 0.1f, 100.0f);	// FOV, aspect ratio based on window dimensions, near and far clip planes

		fpsCam = new FPSCameraController(cam);

		// Create the TriangleRenderer object
		triangle = new TriangleRenderer();

		// Initialise the object
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

		rock->getTransform().setPosition(12, 0, 8);
		sign->getTransform().setPosition(-2, 0, 3);
		crate->getTransform().setPosition(-14, 0, -4);
		fence->getTransform().setPosition(10, 0, -10);
		tree->getTransform().setPosition(9, 0, -12);
		podium->getTransform().setPosition(-8, 0, 7);
		orb->getTransform().setPosition(-8, 0, 7);
		hedgehog->getTransform().setPosition(20, 0, 20);

		std::string skyboxPath = "./textures/skybox_textures/skybox";
		skybox = new SkyboxRenderer(skyboxPath + "_front.png", skyboxPath + "_back.png", skyboxPath + "_right.png", skyboxPath + "_left.png", skyboxPath + "_top.png", skyboxPath + "_bottom.png");

		lastTicks = SDL_GetTicks();

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
		Uint32 now = SDL_GetTicks();
		float deltaTime = (now - lastTicks) / 1000.0f;	// Seconds
		lastTicks = now;

		fpsCam->update(deltaTime, keyStates);

		// Do something for each entity in the vector
		/*for (int i = 0; i < loadedEntities.size(); i++) {

		}*/

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

		// Render skybox first because it should be drawn behind everything
		skybox->draw(cam);

		// Render the triangle
		triangle->draw(cam);

		// Draw each entity in the vector of loaded entities
		for (int i = 0; i < loadedEntities.size(); i++) {
			mr->draw(cam, loadedEntities[i]);
		}

		if (thirdPerson) {
			mr->draw(cam, player);
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
