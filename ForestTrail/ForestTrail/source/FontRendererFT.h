#pragma once
#include <iostream>

#include <map>
#include <array>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace GE {
	struct Character {
		GLuint TextureID;	// ID handle of the glyph texture
		std::array<int, 2> Size;	// Size of glyph
		std::array<int, 2> Bearing;	// Offset from baseline to left/top of glyph
		unsigned int Advance;	// Offset to advance to next glyph
	};

	class FontRendererFT
	{
	public:

		FontRendererFT() = default;
		~FontRendererFT() = default;
		void init();
		void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

	private:
		void setupShaders();

		// Define a map associative data structure to map a character code with a Character struct 
		// - get a character from the text to render and find the corresponding Character
		std::map<char, Character> charactersMap;

		// Vertex buffer
		GLuint vbo;

		// Ids for shader uniforms and sampler
		GLuint vertexLocation;
		GLuint programId;
		GLuint projMatId;
		GLuint samplerId;
		GLuint textColourId;
	};
}



