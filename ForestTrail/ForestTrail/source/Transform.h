#pragma once

#include <glm/glm.hpp>

namespace GE {
	class Transform
	{
	public:
		Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}

		// [Accessors] (getters)
		// Position
		glm::vec3 getPosition() {
			return position;
		}
		// Rotation
		glm::vec3 getRotation() {
			return rotation;
		}
		// Scale
		glm::vec3 getScale() {
			return scale;
		}

		// [Mutators] (setters)
		// Position
		void setPosition(float x, float y, float z) {
			position = glm::vec3(x, y, z);
		}
		// Function overload to set the position using a vector 3
		void setPosition(glm::vec3 pos) {
			position = pos;
		}

		// Rotation
		void setRotation(float rx, float ry, float rz) {
			rotation = glm::vec3(rx, ry, rz);
		}
		// Function overload to set the rotation using a vector 3
		void setRotation(glm::vec3 rot) {
			rotation = rot;
		}

		// Scale
		void setScale(float sx, float sy, float sz) {
			scale = glm::vec3(sx, sy, sz);
		}
		// Function overload to set the scale using a vector 3
		void setScale(glm::vec3 sca) {
			scale = sca;
		}

	private:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
}
