#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

namespace GE {
	class GameEngine {
	public:
		GameEngine();	// Constructor

		virtual ~GameEngine();	// Deconstructor

		bool init();			// Object initialisation
		bool keep_running();	// Indicates whether a user has closed the window/game
		void update();			// This is where the game logic is updated
		void draw();			// This is where a frame is rendered
		void shutdown();		// This is called when the game is ending, it releases objects created during the game

		void setWindowTitle(const char*);

	private:
		SDL_Window* window;

		SDL_GLContext glContext;
	};

	// Helper function
	void display_info_message(const char*);
}
