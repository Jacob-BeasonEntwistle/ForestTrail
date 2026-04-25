#pragma once
#include <SDL.h>
#include <glm/glm.hpp>
#include <string>
#include "Texture.h"

// GUI classes
namespace GE {
	// Thin wrapper around a Texture object for a GUI image
	class GUIImage {
	public:
		GUIImage(int x, int y, Texture* tex) {
			xPos = x;
			yPos = y;

			img = tex;
		}
		~GUIImage() {
			delete img;
		}

		void setTexture(Texture* tex) {
			if (img) {
				delete img;

				img = nullptr;
			}

			img = tex;
		}

		// Note, returns nullptr if no text has been set
		Texture* getTexture() {
			return img;
		}

		// Position setters and getters
		void setX(int newX) {
			xPos = newX;
		}

		void setY(int newY) {
			yPos = newY;
		}

		// Get position
		int getX() {
			return xPos;
		}

		int getY() {
			return yPos;
		}

	private:
		Texture* img;

		int xPos, yPos;
	};

	// Main GUI rendering class
	class GUIRenderer
	{
	public:
		GUIRenderer(int window_width, int window_height);

		// Initialises GUI shaders and camera
		void init();

		// Draw image of GUI
		void drawImage(GUIImage* guiImg) {
			drawTexture(guiImg->getX(), guiImg->getY(), guiImg->getTexture());
		}

		// Shutdown
		void destroy();

	private:
		void drawTexture(int x, int y, Texture* tex);

	private:
		// Member fields
		// Window dimensions
		int w, h;
		 
		// This member stores the program object that contains the shaders
		GLuint programId;

		// This member stores the attribute to select into the pipeline
		// to link the triangle vertices to the pipeline
		GLint vertexLocation;

		// Link to where vertex uv in a model
		GLint vertexUVLocation;

		// GLSL uniform variables for projection and texture sampler
		GLuint projectionUniformId;
		GLuint samplerId;

		// Project matrix.  Calculated in init and reused in draw
		glm::mat4 projectionMat;
	};
}
