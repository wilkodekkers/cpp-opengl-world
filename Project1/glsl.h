#pragma once

#include <GL/glew.h>
#include <fstream>

using namespace std;

class glsl
{
	static char* contents_;
public:
	glsl();
	~glsl();
	static char* read_file(const char* filename);
	static bool compiled_status(GLint shader_id);
	static GLuint make_vertex_shader(const char* shader_source);
	static GLuint make_fragment_shader(const char* shader_source);
	static GLuint make_shader_program(GLuint vertex_shader_id, GLuint fragment_shader_id);
};

