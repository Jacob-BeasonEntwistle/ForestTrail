#pragma once
#include "Camera.h"
#include "Particle.h"
#include <vector>

namespace GE {
	class ParticleSystem
	{
	public:
		ParticleSystem();

		~ParticleSystem() {
			glDeleteBuffers(1, &vboQuad);
		}

		void init();

		void draw(Camera*);

		void update(float deltaTime) {
			for (Particle* p : particles) {
				p->update(deltaTime);
			}
		}

		// Stores the particle in the vector
		void addParticle(Particle* p) {
			particles.push_back(p);
		}

	private:
		// Member fields
		// This member stores the program object that contains the shaders
		GLuint programId;

		// This member stores the attribute to select into the pipeline
		// to link the triangle vertices to the pipeline
		GLint vertexLocation;

		// Link to vColour attribute which receives a colour
		// and passes to fColour for fragment shader
		GLint vertexUVLocation;

		// This member stores the triangle vertex buffer object containing the vertices
		// transferred from this code to the graphics memory
		GLuint vboQuad;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;

		// Add a vector for storing particles
		std::vector<Particle*> particles;
	};
}
