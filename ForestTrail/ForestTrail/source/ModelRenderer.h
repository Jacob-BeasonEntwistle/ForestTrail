#pragma once

#include <GL/glew.h>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Entity.h"

namespace GE {
	class ModelRenderer
	{
	public:
		ModelRenderer();
		~ModelRenderer();

		void init();

		// Update method to update the state of the object
		void update();

		void draw(Camera* cam, Entity* entity);

		// Release method to release the objects such as vertex buffer
		void destroy();

	private:
		// This member stores the program object that contains the shaders
		GLuint programId;

		// This member stores the attribute to select the pipeline to link the triangle vertices to the pipeline
		GLint vertexPos3DLocation;

		// Link to vColour attribute which receives the colour value in Vertex
		GLint vertexUVLocation;

		// This member stores the triangle vertex buffer object containing the vertices transferred from this code to graphics memory
		GLuint vboModel;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;

		Entity* entity;
	};
}
