#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FontRendererFT.h"
#include "ShaderUtils.h"

namespace GE {
    void FontRendererFT::init()
    {
        // Initialise freetype library
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "[Error] FREETYPE: Could not init FreeType Library" << std::endl;
            return;
        }

        // Load the font from which we'll create textures for each character
        FT_Face face;
        if (FT_New_Face(ft, "./Oswald-Regular.ttf", 0, &face)) {
            std::cout << "[Error] FREETYPE: Failed to load font" << std::endl;
            return;
        }

        // Create the font shaders
        setupShaders();

        // Define the size of the font
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Disable byte alignment - necessary because the textures only use the red channel which is 1 byte
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Create textures for characters 32 to 128 - Characters are represented by the well known number code ASCII.
        for (unsigned char c = 32; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "[Error] FREETYPE: Failed to load Glyph" << std::endl;
                continue;
            }

            // Generate texture
            GLuint textureName;
            glGenTextures(1, &textureName);
            glBindTexture(GL_TEXTURE_2D, textureName);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Now store character for later use
            Character character = {
                textureName,
                { (int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows },
                { (int)face->glyph->bitmap_left, (int)face->glyph->bitmap_top },
                static_cast<unsigned int>(face->glyph->advance.x)
            };

            // Add the Character to the map for looking up later
            charactersMap.insert(std::pair<char, Character>(c, character));
        }

        // Generate the vertex buffer
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Allocate space for buffer but don't specify initial values
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
        glUseProgram(NULL);

        // Free up font resources used
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void FontRendererFT::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
    {
        // Enable blending give the way the text is rendered using the red channel as alpha
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Create an ortho projection based on the window size
        SDL_Window* window = SDL_GL_GetCurrentWindow();

        int windowWidth, windowHeight;

        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        glm::mat4 projection = glm::ortho(0.0f, float(windowWidth), 0.0f, float(windowHeight));

        // Setup the pipeline with the shaders to use and uniforms
        glUseProgram(programId);

        glUniform3f(textColourId, color.r, color.g, color.b);
        glUniformMatrix4fv(projMatId, 1, GL_FALSE, glm::value_ptr(projection));

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(vertexLocation);
        glVertexAttribPointer(vertexLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(samplerId, 0);

        // Go through characters in string to render
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            // Find the Character data for the character being rendered
            Character ch = charactersMap[*c];

            // Calculate the dimensions of the characters for updating vertex buffer
            float xpos = x + ch.Bearing[0] * scale;
            float ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

            float w = ch.Size[0] * scale;
            float h = ch.Size[1] * scale;
            // Update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos,     ypos + h,     0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };

            // Update the vertex buffer with the vertices representing the character dimensions - basically a quad
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            // Bind texture
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            // Draw character
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // Now advance cursors to the next glyph - bitshift by 6 to get value in pixels (2^6 = 64)
            x += (ch.Advance >> 6) * scale;
        }

        // Release resources
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(NULL);

        glDisable(GL_BLEND);
    }

    // Helper function to get the total width of the text - useful for centering text
    float FontRendererFT::getTextWidth(std::string text, float scale)
    {
        float width = 0.0f;

        // For each character in the string
        for (char c : text) {
            // Get the preloaded glyph data for the character
            Character ch = charactersMap[c];

            // Advance represents horizontal cursor movement in 1/64th pixel units
            // Adding up how far each letter tells you to move to work out width of whole word
            
            // Convert the character to pixel space and apply the scale
            width += (ch.Advance >> 6) * scale;
        }

        return width;
    }

    void FontRendererFT::setupShaders()
    {
        // Load shader code from files using the ShaderUtils class
        std::string v_shader_source = loadShaderSourceCode("./shaders/fontrenderft.vert");
        std::string f_shader_source = loadShaderSourceCode("./shaders/fontrenderft.frag");

        // OpenGL expects an array of strings, create an array of the loaded source code
        const GLchar* v_source_array[] = { v_shader_source.c_str() };
        const GLchar* f_source_array[] = { f_shader_source.c_str() };

        // Compile shaders into a program
        if (!compileProgram(v_source_array, f_source_array, &programId)) {
            std::cerr << "Problem building FontRender FT program.  Check console log for more information." << std::endl;
        }

        vertexLocation = glGetAttribLocation(programId, "vertex");

        projMatId = glGetUniformLocation(programId, "projectionMatrix");
        samplerId = glGetUniformLocation(programId, "text");
        textColourId = glGetUniformLocation(programId, "textColour");
    }
}
