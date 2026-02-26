#pragma once

#include <GL/glew.h>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

namespace GE {
	class ModelRenderer
	{
	public:
		ModelRenderer();
		~ModelRenderer();

		void init();

		// Update method to update the state of the object
		void update();

		void draw(Camera* cam, Model* model);

		// Release method to release the objects such as vertex buffer
		void destroy();

		// [Accessors] (getters)
		// Position
		float getPosX() {
			return pos_x;
		}
		float getPosY() {
			return pos_y;
		}
		float getPosZ() {
			return pos_z;
		}

		// Rotation
		float getRotX() {
			return rot_x;
		}
		float getRotY() {
			return rot_y;
		}
		float getRotZ() {
			return rot_z;
		}

		// Scale
		float getScaleX() {
			return scale_x;
		}
		float getScaleY() {
			return scale_y;
		}
		float getScaleZ() {
			return scale_z;
		}

		// [Mutators] (setters)
		void setPos(float x, float y, float z) {
			pos_x = x;
			pos_y = y;
			pos_z = z;
		}
		void setRotation(float rx, float ry, float rz) {
			rot_x = rx;
			rot_y = ry;
			rot_z = rz;
		}
		void setScale(float sx, float sy, float sz) {
			scale_x = sx;
			scale_y = sy;
			scale_z = sz;
		}

		// Texture methods
		void setTexture(Texture* tex_) {
			tex = tex_;
		}

	private:
		// This member stores the program object that contains the shaders
		GLuint programId;

		// This member stores the attribute to select the pipeline to link the triangle vertices to the pipeline
		GLint vertexPos3DLocation;

		// Link to vColour attribute which receives the colour value in Vertex
		GLint vertexUVLocation;

		// This member stores the triangle vertex buffer object containing the vertices transferred from this code to graphics memory
		GLuint vboModel;

		// Location, rotation and scale variables
		float pos_x, pos_y, pos_z;
		float rot_x, rot_y, rot_z;
		float scale_x, scale_y, scale_z;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;

		Texture* tex;
	};
}
