#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include "ShaderUtils.h"
#include "Camera.h"
#include "FPSCameraController.h"
#include "TriangleRenderer.h"
#include "ModelRenderer.h"
#include "SkyboxRenderer.h"
#include "Entity.h"

namespace GE {
	class GameEngine {
	public:
		GameEngine();	// Constructor
		GameEngine(int _w, int _h, float _FOV);
		virtual ~GameEngine();	// Deconstructor

		bool init();			// Object initialisation
		bool keep_running();	// Indicates whether a user has closed the window/game
		void update();			// This is where the game logic is updated
		void draw();			// This is where a frame is rendered
		void shutdown();		// This is called when the game is ending, it releases objects created during the game

		void setWindowTitle(const char*);
		bool fullscreen = false;
		int w, h;
		int windowflags;

		int FOV;

	private:
		// Members to track ticks between frames
		Uint32 lastTicks = 0;

		SDL_Window* window;

		SDL_GLContext glContext;

		bool vsync = false;		// By default vsync is off

		// [Movement keys & settings]
		// Array to hold states of key presses
		bool keyStates[4] = { 0, 0, 0, 0 };
		// Indices to the array to store respective key presses
		int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3;

		// [Camera & settings]
		Camera* cam;
		// Third person toggle
		bool thirdPerson = false;

		// Camera controllers
		FPSCameraController* fpsCam;

		// Distance/direction to look from camera
		glm::vec3 dist;

		// Entities
		Entity* rock;
		Entity* sign;
		Entity* crate;
		Entity* fence;
		Entity* tree;
		Entity* podium;
		Entity* orb;
		Entity* hedgehog;

		Entity* player;

		// Texture for all Models
		Texture* tex;
		// Blank texture
		Texture* blank;

		// Model renderer
		ModelRenderer* mr;

		// Orb (dynamic model) variables
		float dynamDirection = 1.0f;
		float dynamSpeed = 1.25f;

		// For storing entities
		std::vector<Entity*> loadedEntities;

		// TriangleRenderer object variable
		TriangleRenderer* triangle;
		
		// SkyboxRenderer object variable
		SkyboxRenderer* skybox;
	};

	// Helper function
	void display_info_message(const char*);
}
