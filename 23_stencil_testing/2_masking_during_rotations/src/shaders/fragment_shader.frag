#version 330 core

uniform vec3 color_value;
out vec4 color;

void main()
{
	color = vec4(vec3(color_value), 1.0f);
}

