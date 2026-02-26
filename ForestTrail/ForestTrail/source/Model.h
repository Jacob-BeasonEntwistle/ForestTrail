#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>

namespace GE {
	// Represents a vertex in the engine
	struct Vertex {
		// Location
		float x, y, z;
		float u, v;

		// Constructors
		// Sets vertex to passed values
		Vertex(float x_, float y_, float z_, float u_, float v_) {
			// Location
			x = x_;
			y = y_;
			z = z_;

			// Colour
			u = u_;
			v = v_;
		}

		Vertex() {
			x = y = z = 0.0f;
			u = v = 0.0f;
		}
	};

	// Represents a model mesh in game engine
	class Model
	{
	public:
		// Constructor
		Model() {
			vbo = 0;
			numVertices = 0;
		}

		// Destructor
		~Model() {
			glDeleteBuffers(1, &vbo);
		}

		// Help method to load vertices from a file
		bool loadFromFile(const char* filename);

		// Returns the vertices so we can create a vertex buffer based on the model vertices
		GLuint getVertices() {
			return vbo;
		}

		// Returns the number of vertices in model for rendering as OpenGL needs to know the number of vertices to render
		int getNumVertices() {
			return numVertices;
		}

	private:
		// Member data
		GLuint vbo;
		int numVertices;
	};
}
