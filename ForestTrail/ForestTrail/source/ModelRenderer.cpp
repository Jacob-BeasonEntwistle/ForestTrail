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
		const GLchar* V_ShaderCode[] = {
			"#version 410\n"
			"in vec3 vertexPos3D;\n"
			"in vec2 vUV;\n"
			"out vec2 uv;\n"
			"uniform mat4 transformMat;\n"
			"uniform mat4 viewMat;\n"
			"uniform mat4 projMat;\n"
			"void main() {\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"v = projMat * viewMat * transformMat * v;\n"
			"gl_Position = v;\n"
			"uv = vUV;\n"
			"}\n"
		};

		const GLchar* F_ShaderCode[] = {
			"#version 410\n"
			"in vec2 uv;\n"
			"uniform sampler2D sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main() {\n"
			"fragmentColour = texture(sampler, uv).rgba;\n"
			"}\n"
		};

		// Attaches the vertex shader and fragment shader to the program
		compileProgram(V_ShaderCode, F_ShaderCode, &programId);

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

		// Get uniform id in shader so C++ program can send data to it
		viewUniformId = glGetUniformLocation(programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat");
		// Transformation matrix uniform
		transformUniformId = glGetUniformLocation(programId, "transformMat");
		// Sampler is linked in the same way
		samplerId = glGetUniformLocation(programId, "sampler");
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
