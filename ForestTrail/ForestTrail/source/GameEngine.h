#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include "ShaderUtils.h"
#include "Camera.h"
#include "TriangleRenderer.h"
#include "Model.h"
#include "ModelRenderer.h"

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

		// Camera
		Camera* cam;

		// Models
		Model* rock;
		Model* sign;
		Model* crate;

		// Model textures
		Texture* rockTex;
		Texture* signTex;
		Texture* crateTex;

		// Blank texture
		Texture* tex;

		// Model renderer
		ModelRenderer* mr;

		// For storing models
		std::vector<Model*> loadedModels;

		// TriangleRenderer object variable
		TriangleRenderer* triangle;
	};

	// Helper function
	void display_info_message(const char*);
}
