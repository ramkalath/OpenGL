#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute

void main()
{
	gl_Position = vec4(position, 1.0);
}

