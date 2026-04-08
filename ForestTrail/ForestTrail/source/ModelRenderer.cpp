#include "ModelRenderer.h"
#include <iostream>
#include "Model.h"
#include "ShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>

namespace GE {
	ModelRenderer::ModelRenderer() {
		
	}
	ModelRenderer::~ModelRenderer() {

	}

	// Helper function that shows compiler error messages if the shader doesn't compile
	extern void displayShaderCompilerError(GLuint shaderId);

	// Creates and compiles the shaders, creates the project and links it and creates the vertex buffer object
	void ModelRenderer::init() {
		// Load shader code from files using the ShaderUtils class
		std::string v_shader_source = loadShaderSourceCode("./shaders/model.vert");
		std::string f_shader_source = loadShaderSourceCode("./shaders/model.frag");

		// OpenGL expects an array of strings, create an array of the loaded source code
		const GLchar* v_source_array[] = { v_shader_source.c_str() };
		const GLchar* f_source_array[] = { f_shader_source.c_str() };

		// Attaches the vertex shader and fragment shader to the program
		// Compile shaders into a program
		if (!compileProgram(v_source_array, f_source_array, &programId)) {
			std::cerr << "Problem building instancing program.  Check console log for more information." << std::endl;
		}

		// Get a link to the vertexPos2D to link vertices when rendering
		vertexPos3DLocation = glGetAttribLocation(programId, "vertexPos3D");
		// Check for errors
		if (vertexPos3DLocation == -1) {
			std::cerr << "Problem getting vertexPos3D" << std::endl;
		}

		// Link to the vUV attribute
		vertexUVLocation = glGetAttribLocation(programId, "vUV");
		// Check for errors
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		// Link to the normal attribute
		vertexNormal = glGetAttribLocation(programId, "vertexNormal");
		// Check for errors
		if (vertexNormal == -1) {
			std::cerr << "Problem getting vertexNormal" << std::endl;
		}

		// Get uniform id in shader so C++ program can send data to it
		viewUniformId = glGetUniformLocation(programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat");
		// Transformation matrix uniform
		transformUniformId = glGetUniformLocation(programId, "transformMat");
		// Sampler is linked in the same way
		samplerId = glGetUniformLocation(programId, "sampler");
		viewPosId = glGetUniformLocation(programId, "viewPos");
		lightColourId = glGetUniformLocation(programId, "lightColour");
	}

	void ModelRenderer::update() {

	}

	// Draw renders the triangles from the buffer object
	void ModelRenderer::draw(Camera* cam, Entity* entity) {
		glEnable(GL_CULL_FACE);

		glm::vec3 pos = entity->getTransform().getPosition();
		glm::vec3 rot = entity->getTransform().getRotation();
		glm::vec3 scale = entity->getTransform().getScale();

		//Calculate the transformation matrix for the object
		glm::mat4 transformationMat = glm::mat4(1.0f);

		transformationMat = glm::translate(transformationMat, pos);
		transformationMat = glm::rotate(transformationMat, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMat = glm::scale(transformationMat, scale);

		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		// Select the program into the rendering context
		glUseProgram(programId);

		// Set the uniforms in the shader
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		// Pass the camera position to fragment shader viewPos
		glUniform3f(viewPosId, cam->getPos().x, cam->getPos().y, cam->getPos().z);

		// Pass the light to the fragment shader
		glUniform3f(lightColourId, 1.0f, 1.0f, 1.0f);

		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, entity->getModel()->getVertices());

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexPos3DLocation);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer and store in vertexPos3DLocation attribute
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexUVLocation);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer and store in vUV attribute
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glEnableVertexAttribArray(vertexNormal);

		// Define where pipeline will find vertex normal in a vertex in vertex buffer to put into vertexNormal attribute
		glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));

		// Select the texture
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, entity->getTexture()->getTextureName());

		// Draw the model
		glDrawArrays(GL_TRIANGLES, 0, entity->getModel()->getNumVertices());

		// Unselect the attribute from the context
		glDisableVertexAttribArray(vertexPos3DLocation);

		// Unselect the colour attribute from the pipeline
		glDisableVertexAttribArray(vertexUVLocation);

		// Unselect the normal attribute from the pipeline
		glDisableVertexAttribArray(vertexNormal);

		// Unselect vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Unselect the program from the context
		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}

	// Release objects allocated for program and vertex buffer object
	void ModelRenderer::destroy() {
		glDeleteProgram(programId);

		glDeleteBuffers(1, &vboModel);
	}
}
