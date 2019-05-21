#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute
layout (location = 1) in vec2 tex_coords; // texture_coordinate attribute

layout (std140) uniform Matrices
{
	mat4 view;
	mat4 projection_perspective;
};

uniform mat4 model;

void main()
{
	gl_Position = projection_perspective * view * model * vec4(position, 1.0);
}

