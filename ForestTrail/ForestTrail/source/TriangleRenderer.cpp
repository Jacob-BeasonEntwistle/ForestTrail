#include "TriangleRenderer.h"
#include <iostream>

namespace GE {
	GLfloat vertexData[] = {
		-1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f
	};

	TriangleRenderer::TriangleRenderer() {

	}
	TriangleRenderer::~TriangleRenderer() {

	}

	// Helper function that shows compiler error messages if the shader doesn't compile
	void displayShaderCompilerError(GLuint shaderId) {
		// First get the length of the error message string
		GLint MsgLen = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &MsgLen);

		// Only display if the message is more than 1 character
		if (MsgLen > 1) {
			// Create a character buffer to store the error message characters
			GLchar* Msg = new GLchar[MsgLen + 1];

			// Get the error message characters from the OpenGL internal log into the Msg buffer
			glGetShaderInfoLog(shaderId, MsgLen, NULL, Msg);

			// Display the error message to see the problem
			std::cerr << "Error compiling shader! Error: " << Msg << std::endl;

			// Release the memory allocated to the string
			delete[] Msg;
		}
	}

	// Creates and compiles the shaders, creates the project and links it and creates the vertex buffer object
	void TriangleRenderer::init() {
		// Create the vertex shader first
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* V_ShaderCode[] = {
			"#version 410\n"
			"in vec3 vertexPos2D\n"
			"void main() {\n"
			"gl_Position = vec4(vertexPos2D.x, vertexPos2D.y, 0, 1);\n"
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
			"out vec4 fragmentColour;\n"
			"void main() {\n"
			"fragmentColour = vec4(1.0, 0.0, 0.0, 1.0);\n"
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
		vertexPos2DLocation = glGetAttribLocation(programId, "vertexPos2D");

		// Check for errors
		if (vertexPos2DLocation == -1) {
			std::cerr << "Problem getting vertexPos2D" << std::endl;
		}

		// Create the vertex buffer object
		glGenBuffers(1, &vboTriangle);
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		// Transfer vertices to graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		// Tidy up after setting up buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void TriangleRenderer::update() {

	}

	// Draw renders the triangles from the buffer object
	void TriangleRenderer::draw() {
		// Select the program into the rendering context
		glUseProgram(programId);

		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexPos2DLocation);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer and store in vertexPos2DLocation attribute
		glVertexAttribPointer(vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Unselect the attribute from the context
		glDisableVertexAttribArray(vertexPos2DLocation);

		// Unselect vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Unselect the program from the context
		glUseProgram(0);
	}

	// Release objects allocated for program and vertex buffer object
	void TriangleRenderer::destroy() {
		glDeleteProgram(programId);

		glDeleteBuffers(1, &vboTriangle);
	}
}
