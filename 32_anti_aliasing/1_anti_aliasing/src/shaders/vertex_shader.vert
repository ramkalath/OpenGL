#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection_perspective;

void main()
{
	gl_Position = projection_perspective * view * model * vec4(position, 1.0);
}

