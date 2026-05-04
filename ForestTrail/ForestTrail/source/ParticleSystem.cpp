#include <glm/glm.hpp>
#include <iostream>
#include "ParticleSystem.h"
#include "ShaderUtils.h"
#include "Model.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GE {

	// Particle quad
	Vertex particle[] = {
		Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),

		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		Vertex(0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f)
	};

	ParticleSystem::ParticleSystem()
	{

	}

	void ParticleSystem::init()
	{
		// Load shader code from files using the ShaderUtils class
		std::string v_shader_source = loadShaderSourceCode("./shaders/particle.vert");
		std::string f_shader_source = loadShaderSourceCode("./shaders/particle.frag");

		// OpenGL expects an array of strings, create an array of the loaded source code
		const GLchar* v_source_array[] = { v_shader_source.c_str() };
		const GLchar* f_source_array[] = { f_shader_source.c_str() };

		// Compile shaders into a program
		if (!compileProgram(v_source_array, f_source_array, &programId)) {
			std::cerr << "Problem building particle program.  Check console log for more information." << std::endl;
		}

		// Link to the vertexPos3D
		vertexLocation = glGetAttribLocation(programId, "vertexPos3D");

		// Check for errors
		if (vertexLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		// Link to vUVs
		vertexUVLocation = glGetAttribLocation(programId, "vUV");

		// Check for errors
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		// Link the uniforms to the member fields
		transformUniformId = glGetUniformLocation(programId, "transformMat");
		viewUniformId = glGetUniformLocation(programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat");
		samplerId = glGetUniformLocation(programId, "sampler");

		// Create the vertex buffer for the quad
		glGenBuffers(1, &vboQuad);
		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);

		// Transfer vertices to graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(particle), particle, GL_STATIC_DRAW);
	}

	void ParticleSystem::draw(Camera* cam)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Disable the depth writes for alpha-blended particles 
		// to prevent transparent areas from incorrectly blocking geometry behind them
		glDepthMask(GL_FALSE);

		// Select the program into the rendering context
		glUseProgram(programId);

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexLocation);
		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer
		// and store in vertexPos2DLocation attribute
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexUVLocation);
		// Define where the vertex specification will find the colour data and how much
		// Colour data is four float values, located at where the r member is.  Stride is a vertex apart
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);

		for (Particle* p : particles) {
			// Billboard rotation
			glm::vec3 pPos(p->getX(), p->getY(), p->getZ());	// Gets the position of the particle
			glm::vec3 camPos = cam->getPos();					// Gets the position of the camera
			glm::vec3 diff = camPos - pPos;						// Works out the differnce between the camera and the particle
			float angle = std::atan2(diff.x, diff.z);			// Calculates an angle using the difference

			// Scale the whole quad around its origin, rotate the quad to face the camera (angle - see above), and then translate the quad to the new xyz
			glm::mat4 transformationMat = glm::mat4(1.0f);
			glm::mat4 scale = glm::scale(transformationMat, glm::vec3(p->getScaleX(), p->getScaleY(), 1.0f));
			glm::mat4 rotate = glm::rotate(transformationMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 translate = glm::translate(transformationMat, pPos);

			transformationMat = translate * rotate * scale;

			// Render object in the same way as a regular model
			// Get the view and projection matrices
			glm::mat4 viewMat = cam->getViewMatrix();
			glm::mat4 projectionMat = cam->getProjectionMatrix();

			// Set the uniforms in the shader
			glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
			glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

			glBindTexture(GL_TEXTURE_2D, p->getTexture()->getTextureName());

			// Draw the model
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// Unselect the attribute from the context
		glDisableVertexAttribArray(vertexLocation);
		glDisableVertexAttribArray(vertexUVLocation);

		// Unselect the program from the context
		glUseProgram(0);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}
