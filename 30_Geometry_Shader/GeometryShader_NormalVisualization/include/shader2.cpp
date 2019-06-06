#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "shader2.h"
#include <sstream>

namespace shader2
{
GLchar* get_program_from_file(bool* f_fail, const char* file_path)
{
	std::ifstream f(file_path);
	std::stringstream f_stream;

	// error_check
	if(!f) *f_fail = 1;
	else *f_fail = 0;

	// read all the contents of that file
	f_stream << f.rdbuf();

	// copy the data to a memory in the heap so that after the scope of this function runs out it still remains
	GLchar* code = new GLchar[f_stream.str().length()+1];
	std::memcpy(code, f_stream.str().c_str(), f_stream.str().length()+1);

	// lets close the open files
	f.close();

	return code;
}

GLuint compile_shader(std::string shader_type, const char* code)
{
	GLuint s = glCreateShader(GL_VERTEX_SHADER); // default we assume its a vertex shader

	if(shader_type == "GEOMETRY") // else its a fragment shader
		s = glCreateShader(GL_GEOMETRY_SHADER);

	if(shader_type == "FRAGMENT") // else its a fragment shader
		s = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(s, 1, &code, NULL);
	glCompileShader(s);

	GLint success;
	GLchar errorLog[512];
	glGetShaderiv(s, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(s, 512, NULL, errorLog);
		std::cerr << "ERROR: " << shader_type << " SHADER COMPILATION FAILED. " << errorLog << std::endl;
	}
	return s;
}
}

// constructor --------------------------------------------------------------------------
Normalshader::Normalshader(const char* vs_path, const char* gs_path, const char* fs_path)
{
	bool s_fail;

	// get the vertex shader code
	const GLchar* vs_code = shader2::get_program_from_file(&s_fail, vs_path);
	if(s_fail) std::cerr << "ERROR: VERTEX SHADER FILE COULD NOT BE OPENED" << std::endl;

	// get the geometry shader code
	const GLchar* gs_code = shader2::get_program_from_file(&s_fail, gs_path);
	if(s_fail) std::cerr << "ERROR: GEOMETRY SHADER FILE COULD NOT BE OPENED" << std::endl;

	// get the fragment shader code
	const GLchar* fs_code = shader2::get_program_from_file(&s_fail, fs_path);
	if(s_fail) std::cerr << "ERROR: FRAGMENT SHADER FILE COULD NOT BE OPENED" << std::endl; 

	 //compile vs and fs --------------------------------------------
	GLuint vs = shader2::compile_shader("VERTEX", vs_code);
	GLuint gs = shader2::compile_shader("GEOMETRY", gs_code);
	GLuint fs = shader2::compile_shader("FRAGMENT", fs_code);

	// link the shaders ---------------------------------------------
	// since linking is not very lengthy lets do it here
	this->program = glCreateProgram();
	glAttachShader(this->program, vs);
	glAttachShader(this->program, gs);
	glAttachShader(this->program, fs);
	glLinkProgram(this->program);

	// check for linking errors
	GLint success;
	GLchar errorLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(this->program, 512, NULL, errorLog);
		std::cerr << "ERROR: SHADER PROGRAM LINKING FAILED" << errorLog << std::endl;
	}

	//---------------------------------------------------------------
    // Delete shaders after compilation
	delete(vs_code);
	delete(gs_code);
	delete(fs_code);
	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);
}

// Destructor for deleting the used program ---------------------------------------------
Normalshader::~Normalshader()
{
	glDeleteShader(program);	
}
