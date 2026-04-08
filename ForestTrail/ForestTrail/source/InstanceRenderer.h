#pragma once
#include <GL/glew.h>
#include <vector>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

namespace GE {
	// Structure for the instance data
	struct InstancePosRotScale {
		float posX, posY, posZ;
		float rotX, rotY, rotZ;
		float scaleX, scaleY, scaleZ;
	};

	class InstanceRenderer
	{
	public:
		InstanceRenderer();
		~InstanceRenderer();

		void init();

		void drawInstanced(Camera* cam, Model* m);

		void destroy();

		float randomFloat(float min, float max);

		InstancePosRotScale setRandomPos(
			float minX, float maxX,
			float minY, float maxY,
			float minZ, float maxZ,
			float minX2, float maxX2,
			float minScale = 1.0f, float maxScale = 1.0f
		);

		int getInstanceCount() {
			return numInstances;
		}

		void setInstanceData(const std::vector<InstancePosRotScale>& instances);
		
		void setTexture(Texture *_tex) {
			tex = _tex;
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

		// Link to normal attribute
		GLint vertexNormal;

		// Link instance matrix attribute
		GLint instanceMatLocation;

		// GLSL uniform variables for the transformation, view and projection matrices
		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint samplerId;
		// For camera position
		GLuint viewPosId;
		// For light colour
		GLuint lightColourId;

		int numInstances;

		GLuint instanceMatrixBuffer;

		Texture *tex;
	};
}

