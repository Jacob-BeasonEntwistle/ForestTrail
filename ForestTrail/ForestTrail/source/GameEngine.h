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
#include "SkyboxRenderer.h"

namespace GE {
	class GameEngine {
	public:
		GameEngine();	// Constructor
		GameEngine(int _w, int _h, float _FOV);
		virtual ~GameEngine();	// Deconstructor

		bool init();			// Object initialisation
		bool keep_running();	// Indicates whether a user has closed the window/game
		void processInput();	// Process key presses
		void update();			// This is where the game logic is updated
		void draw();			// This is where a frame is rendered
		void shutdown();		// This is called when the game is ending, it releases objects created during the game

		void setWindowTitle(const char*);
		bool fullscreen = false;
		int w, h;
		int windowflags;

		int FOV;

	private:
		SDL_Window* window;

		SDL_GLContext glContext;

		bool vsync = false;		// By default vsync is off

		// Array to hold states of key presses
		bool keyStates[4] = { 0, 0, 0, 0 };
		// Indices to the array to store respective key presses
		int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3;

		// Camera
		Camera* cam;

		// Distance/direction to look from camera
		glm::vec3 dist;

		// Models
		Model* rock;
		Model* sign;
		Model* crate;
		Model* fence;
		Model* tree;
		Model* podium;
		Model* orb;

		// Texture for all models
		Texture* tex;
		// Blank texture
		Texture* blank;

		// Model renderer
		ModelRenderer* mr;

		// Dynamic model renderer [Temporary]
		ModelRenderer* dmr;
		// Dynamic model variables [Temporary]
		float direction = 1.0f;
		float speed = 0.05f;

		// For storing models
		std::vector<Model*> loadedModels;

		// TriangleRenderer object variable
		TriangleRenderer* triangle;
		
		// SkyboxRenderer object variable
		SkyboxRenderer* skybox;
	};

	// Helper function
	void display_info_message(const char*);
}
