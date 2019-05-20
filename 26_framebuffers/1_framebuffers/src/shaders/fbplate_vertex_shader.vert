#version 330 core
layout (location = 0) in vec2 position; // vertex_position attribute
layout (location = 1) in vec2 tex_coords; // texture_coordinate attribute

out vec2 texCoords;

void main()
{
	gl_Position = vec4(position, 1.0, 1.0);
	texCoords = tex_coords;
}

