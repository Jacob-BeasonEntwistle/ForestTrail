#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GE {
	class Camera {
	public:
		Camera(glm::vec3 cam_pos, glm::vec3 cam_forwardDir, glm::vec3 cam_upDir, float fov, float ar, float near, float far) {
			// Initialise the camera variables to the values passed on the parameters
			pos = cam_pos;
			forwardDir = cam_forwardDir;
			up = cam_upDir;

			lookDir = forwardDir;

			// Initialise pitch & yaw
			pitch = 0.0f;
			yaw = 0.0f;

			fovy = fov;
			aspectR = ar;
			nearClip = near;
			farClip = far;

			updateCamMatrices();
		}

		~Camera() {

		}

		// Accessor methods
		// Return the position of the camera
		glm::vec3 getPos() {
			return pos;
		}

		glm::vec3 getForwardDir() {
			return forwardDir;
		}

		glm::vec3 getLookDir() {
			return lookDir;
		}

		glm::vec3 getUpDir() {
			return up;
		}

		// Get pitch and yaw
		float getPitch() {
			return pitch;
		}
		float getYaw() {
			return yaw;
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
		// Set position for all axes in one method
		void setPos(glm::vec3 newPos) {
			pos = newPos;

			updateCamMatrices();
		}

		void setPitch(float newPitch) {
			pitch = newPitch;
			// Clamp the pitch values prevent looking straight down/up
			if (pitch > 70.0f) pitch = 70.0f;
			if (pitch < -70.0f) pitch = -70.0f;

			updateCamMatrices();
		}

		void setYaw(float newYaw) {
			yaw = newYaw;

			updateCamMatrices();
		}

		// Set new target
		void setForwardDir(glm::vec3 newDir) {
			forwardDir = newDir;

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

		// Set the aspect ratio
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
		// Member variables
		// Camera view variables
		glm::vec3 pos;
		glm::vec3 forwardDir;
		glm::vec3 lookDir;
		glm::vec3 up;

		// Camera orientation
		float pitch, yaw;

		// Camera projection variables
		float fovy;
		float aspectR;
		float nearClip;
		float farClip;

		// View and projection matrices
		glm::mat4 viewMat;
		glm::mat4 projectionMat;

		// Update the camera matrices when a member variable is updated
		void updateCamMatrices() {
			// Cam matrix taking yaw and pitch into account
			glm::mat4 rollpitchyawMat = glm::mat4(1.0f);

			// Calculate rotation matrix (yaw is negative because OpenGL rotations are counter clockwise)
			rollpitchyawMat = glm::rotate(rollpitchyawMat, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
			rollpitchyawMat = glm::rotate(rollpitchyawMat, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

			// Calculate new look at direction based on the forward direction
			lookDir = rollpitchyawMat * glm::vec4(forwardDir, 0.0f);

			viewMat = glm::lookAt(pos, pos + lookDir, up);
			projectionMat = glm::perspective(glm::radians(fovy), aspectR, nearClip, farClip);
		}
	};
}
