#ifndef NORMALSHADER_H
#define NORMALSHADER_H

#define GLEW_STATIC
#include <GL/glew.h>

class Normalshader
{
public:
	GLuint program;

	// constructor - actual definition of methods in shader.cpp in source directory
	Normalshader(const char* vertex_shader_path, const char* geometry_shader_path, const char* fragment_shader_path);
	~Normalshader();
};

#endif
