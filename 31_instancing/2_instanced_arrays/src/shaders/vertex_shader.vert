#version 330 core
layout (location=0) in vec2 position; // vertex_position attribute
layout (location=1) in vec3 color; // vertex_position attribute
layout (location=2) in vec2 offset; // vertex_position attribute

out vec3 fColor;

void main()
{
	gl_Position = vec4(position + offset, 0.0f, 1.0f);;
	fColor = color;
}

