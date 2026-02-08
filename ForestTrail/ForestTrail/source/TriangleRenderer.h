#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

namespace GE {
	class TriangleRenderer
	{
	public:
		TriangleRenderer();
		virtual ~TriangleRenderer();

		// Init method to create shaders & vertex buffer object
		void init();

		void update();

		void draw();

		// Release method to release the objects such as vertex buffer
		void destroy();

	private:
		// This member stores the program object that contains the shaders
		GLuint programId;

		// This member stores the attribute to select the pipeline to link the triangle vertices to the pipeline
		GLint vertexPos2DLocation;

		// This member stores the triangle vertex buffer object containing the vertices transferred from this code to graphics memory
		GLuint vboTriangle;
	};
}
