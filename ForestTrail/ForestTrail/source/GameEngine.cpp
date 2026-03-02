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

		// Create the TriangleRenderer object
		triangle = new TriangleRenderer();

		// Initialise the object
		triangle->init();
		
		triangle->setPos(0.0f, 0.0f, 0.0f);
		triangle->setRotation(0.0f, 45.0f, 0.0f);
		triangle->setScale(100.0f, 100.0f, 100.0f);

		// Create the texture
		tex = new Texture("./texture_for_models.png");

		rock = new Model();
		rock->loadFromFile("./models/rock.obj");

		sign = new Model();
		sign->loadFromFile("./models/sign.obj");

		crate = new Model();
		crate->loadFromFile("./models/crate.obj");

		// Put all of the models in the vector
		loadedModels.push_back(rock);
		loadedModels.push_back(sign);
		loadedModels.push_back(crate);

		// Create the Model renderer object
		mr = new ModelRenderer();

		mr->init();
		mr->setTexture(tex);

		std::string skyboxPath = "./textures/skybox_textures/skybox";
		skybox = new SkyboxRenderer(skyboxPath + "_front.png", skyboxPath + "_back.png", skyboxPath + "_right.png", skyboxPath + "_left.png", skyboxPath + "_top.png", skyboxPath + "_bottom.png");

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

	void GameEngine::processInput() {
		float mouseSens = cam->getMouseSens();
		float camSpeed = cam->getCamSpeed();

		// Get current mouse x and y
		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		// Calculate the difference between old mouse and new mouse position - note inversion on y as this is flipped
		int diffx = mouse_x - (w / 2);
		int diffy = (h / 2) - mouse_y;

		// Update the yaw and pitch based on the mouse differential
		cam->setYaw(cam->getYaw() + diffx * mouseSens);
		cam->setPitch(cam->getPitch() + diffy * mouseSens);

		// Set the mouse back to the centre
		SDL_WarpMouseInWindow(window, w / 2, h / 2);

		// Calculate new camera facing direction from the change to yaw and pitch
		glm::vec3 direction;
		direction.x = cos(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		direction.y = sin(glm::radians(cam->getPitch()));
		direction.z = sin(glm::radians(cam->getYaw())) * cos(glm::radians(cam->getPitch()));
		cam->setTarget(glm::normalize(direction));

		// Handle camera movement based upon the relevant key press
		if (keyStates[UP]) {
			cam->setPos(cam->getPos() + cam->getTarget() * camSpeed);
		}
		if (keyStates[DOWN]) {
			cam->setPos(cam->getPos() - cam->getTarget() * camSpeed);
		}
		if (keyStates[LEFT]) {
			// Take away the cross product of the facing and up vector from the camera position - strafe left
			cam->setPos(cam->getPos() - glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed);
		}
		if (keyStates[RIGHT]) {
			// Take away the cross product of the facing and up vector from the camera position - strafe right
			cam->setPos(cam->getPos() + glm::normalize(glm::cross(cam->getTarget(), cam->getUpDir())) * camSpeed);
		}

		// Call to update camera view matrices
		// Projection is also updated in case effects like zoom are used (e.g. right click to aim would shrink the fov slightly)
		cam->updateCamMatrices();
	}

	// Update method which updates the game logic
	void GameEngine::update() {
		// Do something for each model in the vector
		/*for (int i = 0; i < loadedModels.size(); i++) {

		}*/
	}

	// Draw method that renders the scene
	void GameEngine::draw() {
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render skybox first because it should be drawn behind everything
		skybox->draw(cam);

		// Render the triangle
		triangle->draw(cam);

		// Draw each model in the vector of loaded models
		for (int i = 0; i < loadedModels.size(); i++) {
			mr->draw(cam, loadedModels[i]);
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

		// Delete each model from the vector of models
		for (int i = 0; i < loadedModels.size(); i++) {
			delete loadedModels[i];
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
