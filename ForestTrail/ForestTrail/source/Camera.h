#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GE {
	class Camera {
	public:
		Camera(glm::vec3 cam_pos, glm::vec3 cam_target, glm::vec3 cam_upDir, float fov, float ar, float near, float far) {
			// Initialise the camera variables to the values passed on the parameters
			pos = cam_pos;
			target = cam_target;
			up = cam_upDir;

			fovy = fov;
			aspectR = ar;
			nearClip = near;
			farClip = far;

			updateCamMatrices();
		}

		~Camera() {

		}

		// Accessor methods
		// Return the position of the camera in terms of x, y, z
		float getPosX() {
			return pos.x;
		}

		float getPosY() {
			return pos.y;
		}

		float getPosZ() {
			return pos.z;
		}

		// Target or what location the camera is looking at
		const glm::vec3 getTarget() {
			return target;
		}

		// Up direction
		glm::vec3 getUpDir() const {
			return up;
		}

		// Return the camera's view matrix.  Used by draw
		// method to send view matrix to vertex shader
		glm::mat4 getViewMatrix() const {
			return viewMat;
		}

		// Return camera's projection matrix.  Same reason
		// as above
		glm::mat4 getProjectionMatrix() const {
			return projectionMat;
		}


		// Mutator methods
		// Set position
		void setPosX(float newX) {
			pos = glm::vec3(newX, pos.y, pos.z);

			updateCamMatrices();
		}

		void setPosY(float newY) {
			pos = glm::vec3(pos.x, newY, pos.z);

			updateCamMatrices();
		}

		void setPosZ(float newZ) {
			pos = glm::vec3(pos.x, pos.y, newZ);

			updateCamMatrices();
		}

		// Set position for all axes in one method
		void setPos(float newX, float newY, float newZ) {
			pos = glm::vec3(newX, newY, newZ);

			updateCamMatrices();
		}

		// Set new target
		void setTarget(glm::vec3 newTarget) {
			target = newTarget;

			updateCamMatrices();
		}

		// Set the up direction
		void setUpDir(glm::vec3 newUp) {
			up = newUp;

			updateCamMatrices();
		}

		// Set the fov
		void setFov(float newFov) {
			fovy = newFov;

			updateCamMatrices();
		}

		// Set the aspect ratio.
		// You might use this method if the program's window
		// can be resized meaning the width and height change
		void setAspectRatio(float newAR) {
			aspectR = newAR;

			updateCamMatrices();
		}

		// Set clipping planes
		void setNearClip(float newNearClip) {
			nearClip = newNearClip;

			updateCamMatrices();
		}

		void setFarClip(float newFarClip) {
			farClip = newFarClip;

			updateCamMatrices();
		}

	private:
		// Update the camera matrices done in response to
		// a member variable variable being updated
		void updateCamMatrices() {
			viewMat = glm::lookAt(pos, target, up);
			projectionMat = glm::perspective(glm::radians(fovy), aspectR, nearClip, farClip);
		}

	private:
		// Member variables
		// Camera view variables
		glm::vec3 pos;
		glm::vec3 target;
		glm::vec3 up;

		// Camera projection variables
		float fovy;
		float aspectR;
		float nearClip;
		float farClip;

		// View and projection matrices
		glm::mat4 viewMat;
		glm::mat4 projectionMat;
	};
}
