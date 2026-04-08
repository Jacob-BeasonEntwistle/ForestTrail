#include "InstanceRenderer.h"
#include "ShaderUtils.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GE {
	InstanceRenderer::InstanceRenderer() {
		tex = nullptr;
	}

	InstanceRenderer::~InstanceRenderer() {

	}

	void InstanceRenderer::init()
	{
		// Load shader code from files using the ShaderUtils class
		std::string v_shader_source = loadShaderSourceCode("./shaders/instance.vert");
		std::string f_shader_source = loadShaderSourceCode("./shaders/instance.frag");

		// OpenGL expects an array of strings, create an array of the loaded source code
		const GLchar* v_source_array[] = { v_shader_source.c_str() };
		const GLchar* f_source_array[] = { f_shader_source.c_str() };

		// Compile shaders into a program
		if (!compileProgram(v_source_array, f_source_array, &programId)) {
			std::cerr << "Problem building instancing program.  Check console log for more information." << std::endl;
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

		// Link to the normal attribute
		vertexNormal = glGetAttribLocation(programId, "vertexNormal");
		// Check for errors
		if (vertexNormal == -1) {
			std::cerr << "Problem getting vertexNormal" << std::endl;
		}

		// Get location of the instance attribute which is the transformation matrix  
		instanceMatLocation = glGetAttribLocation(programId, "instance_transform_mat");
		// Check for errors
		if (instanceMatLocation == -1) {
			std::cerr << "Problem getting instance_transform_mat";
		}

		// Link the uniforms to the member fields
		viewUniformId = glGetUniformLocation(programId, "viewMat");
		projectionUniformId = glGetUniformLocation(programId, "projMat");
		samplerId = glGetUniformLocation(programId, "sampler");
		viewPosId = glGetUniformLocation(programId, "viewPos");
		lightColourId = glGetUniformLocation(programId, "lightColour");
	}

	void InstanceRenderer::setInstanceData(const std::vector<InstancePosRotScale>& instances) {
		// Vector to store the transformation matrices
		std::vector<glm::mat4> instance_matrices;

		// Process each location, rotation and scales from instances calculating 
		// corresponding transformation matrix and insert into vector
		for (auto& iprs : instances) {
			// Calculate the transformation matrix for the object.  Start with the identity matrix
			glm::mat4 instanceMat = glm::mat4(1.0f);

			// Current instance position, rotation and scale
			instanceMat = glm::translate(instanceMat, glm::vec3(iprs.posX, iprs.posY, iprs.posZ));
			instanceMat = glm::rotate(instanceMat, glm::radians(iprs.rotX), glm::vec3(1.0f, 0.0f, 0.0f));
			instanceMat = glm::rotate(instanceMat, glm::radians(iprs.rotY), glm::vec3(0.0f, 1.0f, 0.0f));
			instanceMat = glm::rotate(instanceMat, glm::radians(iprs.rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
			instanceMat = glm::scale(instanceMat, glm::vec3(iprs.scaleX, iprs.scaleY, iprs.scaleZ));

			instance_matrices.push_back(instanceMat);
		}

		// Store the number of instances as need that number in draw method
		numInstances = instance_matrices.size();

		// Setup input of an array of matrix which define the position of each instance
		glGenBuffers(1, &instanceMatrixBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
		glBufferData(GL_ARRAY_BUFFER, instance_matrices.size() * sizeof(glm::mat4), instance_matrices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Draw objects using instancing
	void InstanceRenderer::drawInstanced(Camera* cam, Model* m) {
		// Enable back face culling (other culling types: GL_FRONT, GL_BACK or GL_FRONT_AND_BACK)
		glEnable(GL_CULL_FACE);

		// Get the view and projection matrices
		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		// Select the program into the rendering context
		glUseProgram(programId);

		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		// Pass the camera position to fragment shader viewPos
		glUniform3f(viewPosId, cam->getPos().x, cam->getPos().y, cam->getPos().z);

		// Pass the light to the fragment shader
		glUniform3f(lightColourId, 1.0f, 1.0f, 1.0f);

		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, m->getVertices());

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexLocation);

		// Define the structure of a vertex for OpenGL to select values from vertex buffer
		// and store in vertexPos2DLocation attribute
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexUVLocation);

		// Define where the vertex specification will find the colour data and how much
		// Colour data is four float values, located at where the r member is.  Stride is a vertex apart
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glEnableVertexAttribArray(vertexNormal);

		// Define where pipeline will find vertex normal in a vertex in vertex buffer to put into vertexNormal attribute
		glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));

		// Unbind the vertex buffer before defining the input for the
		// transformation matrices instance buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Bind the matrix to the input specification which starts at instance_transform_mat
		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);

		// Define the layout of the instance matrix input attribute in terms of
		// four vec4 values.  Reason is can't define an input in more than a vec4 so
		// matrix has be broken down into four vec4s
		glEnableVertexAttribArray(instanceMatLocation);
		glVertexAttribPointer(instanceMatLocation, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(instanceMatLocation + 1);
		glVertexAttribPointer(instanceMatLocation + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(instanceMatLocation + 2);
		glVertexAttribPointer(instanceMatLocation + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(instanceMatLocation + 3);
		glVertexAttribPointer(instanceMatLocation + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		// Define how the instance data is used in terms of how many instances
		// pass before the transformation matrix is updated.  1 means one instance
		// or one pass of all of the vertices in the model.  2 means after two
		// passes and so on.  0 means every vertex which is normal behaviour
		glVertexAttribDivisor(instanceMatLocation, 1);
		glVertexAttribDivisor(instanceMatLocation + 1, 1);
		glVertexAttribDivisor(instanceMatLocation + 2, 1);
		glVertexAttribDivisor(instanceMatLocation + 3, 1);

		// Unbind the instance buffer as may want to define more inputs
		// based on another buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Select the texture
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);

		if (!tex) {
			std::cerr << "InstanceRenderer: texture not set!" << std::endl;
			return;
		}

		glBindTexture(GL_TEXTURE_2D, tex->getTextureName());

		// Draw the instances
		glDrawArraysInstanced(GL_TRIANGLES, 0, m->getNumVertices(), numInstances);

		glDisableVertexAttribArray(vertexLocation);
		glDisableVertexAttribArray(vertexUVLocation);

		// Unselect the normal attribute from the pipeline
		glDisableVertexAttribArray(vertexNormal);

		// Set attributes back to per vertex selection
		glVertexAttribDivisor(instanceMatLocation, 0);
		glVertexAttribDivisor(instanceMatLocation + 1, 0);
		glVertexAttribDivisor(instanceMatLocation + 2, 0);
		glVertexAttribDivisor(instanceMatLocation + 3, 0);

		// Unselect the attribute from the context
		glDisableVertexAttribArray(vertexLocation);
		glDisableVertexAttribArray(vertexUVLocation);
		glDisableVertexAttribArray(instanceMatLocation);
		glDisableVertexAttribArray(instanceMatLocation + 1);
		glDisableVertexAttribArray(instanceMatLocation + 2);
		glDisableVertexAttribArray(instanceMatLocation + 3);

		// Unselect the program from the context
		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}

	float InstanceRenderer::randomFloat(float min, float max) {
		// rand gives int between 0 and RAND_MAX
		// Dividing by RAND_MAX normalizes it to [0.0, 1.0]
		// Multiply it by the range size (e.g. (-30) - (-50) = 20)
		// Then add the minimum to shift it into the correct range
		return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
	}

	InstancePosRotScale InstanceRenderer::setRandomPos(
		float minX, float maxX,
		float minY, float maxY,
		float minZ, float maxZ,
		float minX2, float maxX2,
		float minScale, float maxScale
	) {
		// Pick a side at random, either left (0) or right (1)
		int side = rand() % 2;

		// Temporary float variable for pos on x axis
		float x;

		// If the side chosen is the left set x pos on the left
		if (side == 0) {
			x = randomFloat(minX, maxX);
		}
		// If the side chosen is the right set x pos on the right
		else {
			x = randomFloat(minX2, maxX2);
		}

		float y = randomFloat(minY, maxY);

		// Randomise the z pos for each tree
		float z = randomFloat(minZ, maxZ);
		// Randomise the scale of the tree between 0.65 and 1.3
		float scale = randomFloat(minScale, maxScale);

		// Create an instance of the tree with the randomised coordinates and scale
		return InstancePosRotScale{ x, y, z, 0.0f, 0.0f, 0.0f, scale, scale, scale };
	}

	// Release objects allocated for program and vertex buffer object
	void InstanceRenderer::destroy()
	{
		// Delete the instance buffer when finished
		glDeleteBuffers(1, &instanceMatrixBuffer);

		glDeleteProgram(programId);
	}
}
