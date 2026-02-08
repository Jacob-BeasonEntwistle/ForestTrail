#include "GameEngine.h"
#include <iostream>

namespace GE {
	GameEngine::GameEngine() {

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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		// Set the type of profile which is core meaning modern OpenGL
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Create the window and frame features
		// (this has a fixed size and prepares window for OpenGL to render into)
		window = SDL_CreateWindow("SDL OpenGL", 50, 50, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

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
		if (SDL_GL_SetSwapInterval(1) != 0) {
			std::cerr << "Warning: unable to set VSync! Error: " << SDL_GetError() << std::endl;

			return false;
		}

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

		return true;
	}

	// Update method which updates the game logic
	void GameEngine::update() {

	}

	// Draw method that renders the scene
	void GameEngine::draw() {
		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing a 2D triangle
		glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.5f, 0.5f);
			glVertex2f(-1.0f, 0.0f);
			glColor3f(1.0f, 0.5f, 0.5f);
			glVertex2f(1.0f, 0.0f);
			glColor3f(1.0f, 0.5f, 0.5f);
			glVertex2f(0.0f, 1.0f);
		glEnd();

		SDL_GL_SwapWindow(window);
	}

	// Shutdown method closes OpenGL and SDL as well as destroying objects
	void GameEngine::shutdown() {
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
