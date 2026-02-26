#include "ModelRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"

namespace GE {
	ModelRenderer::ModelRenderer() {
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = scale_y = scale_z = 1.0f;
	}
	ModelRenderer::~ModelRenderer() {

	}

	// Helper function that shows compiler error messages if the shader doesn't compile
	extern void displayShaderCompilerError(GLuint shaderId);

	// Creates and compiles the shaders, creates the project and links it and creates the vertex buffer object
	void ModelRenderer::init() {
		// Create the vertex shader first
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

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

		// Copy the source to OpenGL ready for compilation
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);

		// Compile the code
		glCompileShader(vertexShader);

		// Check for compiler errors
		GLint isShaderCompiledOK = GL_FALSE;

		// Get the compile status from OpenGL
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		// Has the shader failed to compile?
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile vertex shader" << std::endl;

			displayShaderCompilerError(vertexShader);

			return;
		}

		// Do the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar* F_ShaderCode[] = {
			"#version 410\n"
			"in vec2 uv;\n"
			"uniform sampler2D sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main() {\n"
			"fragmentColour = texture(sampler, uv).rgba;\n"
			"}\n"
		};

		// Transfer the shader code
		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);

		// Compile it
		glCompileShader(fragmentShader);

		// Check for errors
		isShaderCompiledOK = GL_FALSE;

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile fragment shader" << std::endl;

			displayShaderCompilerError(fragmentShader);

			return;
		}

		// Create the program object
		programId = glCreateProgram();

		// Attach shaders to the program object
		glAttachShader(programId, vertexShader);
		glAttachShader(programId, fragmentShader);

		// Link the problem to create an executable program and use it to render the object
		// Program executable will exist in graphics memory
		glLinkProgram(programId);

		// Check for linking errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;
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
	void ModelRenderer::draw(Camera* cam, Model* model) {
		glEnable(GL_CULL_FACE);

		// Calculate the transformation matrix for the object
		glm::mat4 transformationMat = glm::mat4(1.0f);

		transformationMat = glm::translate(transformationMat, glm::vec3(pos_x, pos_y, pos_z));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMat = glm::scale(transformationMat, glm::vec3(scale_x, scale_y, scale_z));

		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		// Select the program into the rendering context
		glUseProgram(programId);

		// Set the uniforms in the shader
		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, model->getVertices());

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexPos3DLocation);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer and store in vertexPos3DLocation attribute
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexUVLocation);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer and store in vUV attribute
		glVertexAttribPointer(vertexUVLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		// Select the texture
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, tex->getTextureName());

		// Draw the model
		glDrawArrays(GL_TRIANGLES, 0, model->getNumVertices());

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
