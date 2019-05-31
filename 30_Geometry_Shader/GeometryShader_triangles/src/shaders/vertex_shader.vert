#version 330 core
layout (location=0) in vec3 position; // vertex_position attribute
layout (location=1) in vec3 color; // vertex_position attribute

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection_perspective;

out vec3 vColor;

void main()
{
	gl_Position = projection_perspective * view * model * vec4(position, 1.0);
	vColor = color;
}

