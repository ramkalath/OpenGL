#version 330 core

out vec4 color;
in COLOUR
{
	vec4 colour;
} var_out;

void main()
{
	color = var_out.colour;
}

