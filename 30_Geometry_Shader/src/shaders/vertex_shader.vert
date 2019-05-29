#version 330 core
layout (location=0) in vec3 position; // vertex_position attribute

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection_perspective;

out COLOUR
{
	vec4 colour;
} var_in;

void main()
{
	gl_Position = projection_perspective * view * model * vec4(position, 1.0);
	var_in.colour = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}

