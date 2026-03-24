#pragma once

#include <SDL.h>
#include <iostream>
#include "Camera.h"
#include "Entity.h"

namespace GE {
	class ThirdPersonController {
	public:
		ThirdPersonController(Camera* cam, Entity* p) {
			// Get the camera object
			camera = cam;

			player = p;

			oldTicks = SDL_GetTicks();

			camOffset = glm::vec3(0.5f, 6.0f, 5.0f);

			// Get the window dimensions and work out the centre
			SDL_Window* window = SDL_GL_GetCurrentWindow();

			int w, h;

			SDL_GetWindowSize(window, &w, &h);

			// Set mouse to middle of the window
			SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), w / 2, h / 2);
		}

		~ThirdPersonController() {

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

			player->getTransform().setRotation(0.0f, camera->getYaw(), 0.0f);

			// Constrains player to flat plane (player movement parallel to the ground)
			glm::vec3 fwd = camera->getLookDir();
			fwd.y = 0.0f;

			// Normalizing directions prevents unintentional increased movement speed
			fwd = glm::normalize(fwd);
			const glm::vec3 up = glm::normalize(camera->getUpDir());
			const glm::vec3 right = glm::normalize(glm::cross(fwd, up));

			glm::vec3 pos = player->getTransform().getPosition();

			// Move in direction based on keyState pressed and time elapsed since last update
			if (keyStates[0]) {
				// Move forwards
				pos += fwd * (moveSpeed * deltaTime);
			}
			if (keyStates[1]) {
				// Move backwards
				pos -= fwd * (moveSpeed * deltaTime);
			}
			if (keyStates[2]) {
				// Strafe left
				pos -= right * (moveSpeed * deltaTime);
			}
			if (keyStates[3]) {
				// Strafe right
				pos += right * (moveSpeed * deltaTime);
			}

			player->getTransform().setPosition(pos);

			// Identity matrix for starting the rotation transform
			glm::mat4 rot = glm::mat4(1.0f);

			// Creating a rotation matrix around the Y axis (OpenGL rotations are counter-clockwise, hence the -camera->getYaw())
			rot = glm::rotate(rot, glm::radians(-camera->getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));

			// Converts to a 4D vector so it can be multiplied by the 4x4 rotation matrix
			// Multiplying by rot rotates the offset around the Y axis so camera always stays behind the player
			glm::vec4 offset4 = rot * glm::vec4(camOffset, 1.0f);
			glm::vec3 rotatedOffset = glm::vec3(offset4);
			
			// Adds the rotated offset to position the camera behind and above the player
			camera->setPos(player->getTransform().getPosition() + rotatedOffset);

			// Set mouse to middle of window
			SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), w / 2, h / 2);
		}

	private:
		Camera* camera;
		Entity* player;

		Uint32 oldTicks;

		// Mouse sensitivity
		float mouseSens = 0.1f;
		// Movement speed
		const float moveSpeed = 15.0f;

		// Camera offset for thirdperson view
		glm::vec3 camOffset;
	};
}
