#pragma once
#include <GL/glew.h>
#include <string>

namespace GE {
	std::string loadShaderSourceCode(std::string filename);
	bool compileProgram(const char* v_shader_sourcecode[], const char* f_shader_sourcecode[], GLuint *programId);
}
