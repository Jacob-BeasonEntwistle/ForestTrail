#pragma once

#include <SDL.h>
#include <iostream>
#include "Camera.h"

namespace GE {
	class FPSCameraController {
	public:
		FPSCameraController(Camera* cam) {
			// Get the camera object
			camera = cam;

			oldTicks = SDL_GetTicks();

			// Get the window dimensions and work out the centre
			SDL_Window* window = SDL_GL_GetCurrentWindow();

			int w, h;

			SDL_GetWindowSize(window, &w, &h);

			// Set mouse to middle of the window
			SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), w / 2, h / 2);
		}

		~FPSCameraController() {

		}

		void update(float deltaTime, bool* keyStates) {
			// Work out midpoint of window
			SDL_Window* window = SDL_GL_GetCurrentWindow();
			int w, h;
			SDL_GetWindowSize(window, &w, &h);

			// Get mouse position
			int mouse_x, mouse_y;
			SDL_GetMouseState(&mouse_x, &mouse_y);

			// Calculate the deflection (unintentional movement)
			int diffx = mouse_x - (w / 2);
			int diffy = (h / 2) - mouse_y;

			// Update the yaw and pitch based on the mouse deflection
			camera->setYaw(camera->getYaw() + diffx * mouseSens);
			camera->setPitch(camera->getPitch() + diffy * mouseSens);

			// Normalizing directions prevents unintentional increased movement speed
			const glm::vec3 fwd = glm::normalize(camera->getLookDir());
			const glm::vec3 up = glm::normalize(camera->getUpDir());
			const glm::vec3 right = glm::normalize(glm::cross(fwd, up));

			// Move in direction based on keyState pressed and time elapsed since last update
			if (keyStates[0]) {
				camera->setPos(camera->getPos() + fwd * (moveSpeed * deltaTime));
			}
			if (keyStates[1]) {
				camera->setPos(camera->getPos() - fwd * (moveSpeed * deltaTime));
			}
			if (keyStates[2]) {
				// Take away the cross product of the facing and up vector from the camera position - strafe left
				camera->setPos(camera->getPos() - right * (moveSpeed * deltaTime));
			}
			if (keyStates[3]) {
				// Add the cross product of the facing and up vector from the camera position - strafe right
				camera->setPos(camera->getPos() + right * (moveSpeed * deltaTime));
			}

			// Set mouse to middle of window
			SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), w / 2, h / 2);
		}

	private:
		Camera* camera;
		Uint32 oldTicks;
		
		// Mouse sensitivity
		float mouseSens = 0.1f;
		// Movement speed
		const float moveSpeed = 15.0f;
	};
}
