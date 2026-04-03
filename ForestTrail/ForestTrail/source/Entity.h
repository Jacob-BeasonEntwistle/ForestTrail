#pragma once

#include "Model.h"
#include "Texture.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <iostream>

namespace GE {
	class Entity
	{
	public:
		Entity(const char* modelFile, Texture* texture = nullptr) : transform() {
			model = new Model();
			model->loadFromFile(modelFile);

			if (texture) {
				tex = texture;
			}
			else {
				tex = new Texture("./textures/blank_texture.png");
			}
		}

		// Getters for the transform, model, and texture
		Transform& getTransform() { return transform; }
		Model* getModel() { return model; }
		Texture* getTexture() { return tex; }
		std::string getName() { return name; }

		// Setters for the model and texture
		void setModel(Model* m) {
			model = m;
		}
		void setTexture(Texture* t) {
			tex = t;
		}
		void setName(const std::string& n) {
			name = n;
		}

	private:
		Model* model;
		Texture* tex;
		Transform transform;
		std::string name;
	};
}
