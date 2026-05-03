#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <sstream>
#include "ShaderUtils.h"
#include "Camera.h"
#include "FPSCameraController.h"
#include "ThirdPersonController.h"
#include "TriangleRenderer.h"
#include "ModelRenderer.h"
#include "SkyboxRenderer.h"
#include "Entity.h"
#include "FontRendererFT.h"
#include "InstanceRenderer.h"
#include "GUIRenderer.h"
#include "ParticleSystem.h"
#include "MathUtils.h"

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
		
		// Members for the fps counter
		Uint32 fpsLastTime = 0;
		int frameCount = 0;
		int currentFPS = 0;
		std::string fpsText;

		SDL_Window* window = nullptr;

		SDL_GLContext glContext = nullptr;

		bool vsync = false;		// By default vsync is off

		// [Movement keys & settings]
		// Array to hold states of key presses
		bool keyStates[4] = { 0, 0, 0, 0 };
		// Indices to the array to store respective key presses
		int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3;

		// [Camera & settings]
		Camera* cam = nullptr;
		// Third person toggle
		bool thirdPerson = false;
		// Stats toggle
		bool showStats = false;
		// Sprinting
		bool isSprinting = false;

		// [Stats variables]
		int numOfEntities = 0;
		int numOfTrees = 0;
		int numOfRocks = 0;
		int totalVertices = 0;
		int totalTriangles = 0;

		// Camera controllers
		FPSCameraController* fpsCam = nullptr;
		ThirdPersonController* thirdCam = nullptr;

		// Distance/direction to look from camera
		glm::vec3 dist = glm::vec3(0.0f);

		// Entities
		Entity* rock = nullptr;
		Entity* sign = nullptr;
		Entity* crate = nullptr;
		Entity* fence = nullptr;
		Entity* tree = nullptr;
		Entity* podium = nullptr;
		Entity* orb = nullptr;
		Entity* hedgehog = nullptr;

		Entity* player = nullptr;

		// Texture for all Models
		Texture* tex = nullptr;
		// Blank texture
		Texture* blank = nullptr;

		// Model renderer
		ModelRenderer* mr = nullptr;

		// Orb (dynamic model) variables
		float dynamDirection = 1.0f;
		float dynamSpeed = 1.25f;

		// For storing entities
		std::vector<Entity*> loadedEntities;

		// TriangleRenderer object variable - Terrain
		TriangleRenderer* triangle = nullptr;
		
		// SkyboxRenderer object variable
		SkyboxRenderer* skybox = nullptr;

		// FontRendererFT object variable
		FontRendererFT* fontFT = nullptr;

		// GUIRenderer object variable
		GUIRenderer* gui = nullptr;
		
		// GUI images
		GUIImage* crosshairImg = nullptr;
		GUIImage* miniMapBackgronudImg = nullptr;
		GUIImage* miniMapImg = nullptr;
		GUIImage* playerIconImg = nullptr;

		// Define minimap bounds
		float minimapSize = 128.0f;
		float minimapPadding = 6.0f;	// Padding to avoid overlapping the background texture
		float worldSize = 70.0f;		// The size of the world terrain

		// InstanceRenderer object variable
		InstanceRenderer* treeIr = nullptr;
		InstanceRenderer* rockIr = nullptr;

		// Vectors used for rendering instances
		std::vector<InstancePosRotScale> treeInstances;
		std::vector<InstancePosRotScale> rockInstances;

		// ParticleSystem object variable
		ParticleSystem* ps;
		Particle* particle;
	};

	// Helper function
	void display_info_message(const char*);
}
