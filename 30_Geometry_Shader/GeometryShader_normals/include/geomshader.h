#ifndef GEOMSHADER_H
#define GEOMSHADER_H

#define GLEW_STATIC
#include <GL/glew.h>

class Geomshader
{
public:
	unsigned int program;

	// constructor - actual definition of methods in shader.cpp in source directory
	Geomshader(const char* vertex_shader_path, const char* geometric_shader_path, const char* fragment_shader_path);
	~Geomshader();
};

#endif
