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
		void setPosition(float x, float y, float z) {
			position = glm::vec3(x, y, z);
		}
		void setRotation(float rx, float ry, float rz) {
			rotation = glm::vec3(rx, ry, rz);
		}
		void setScale(float sx, float sy, float sz) {
			scale = glm::vec3(sx, sy, sz);
		}

	private:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
}
