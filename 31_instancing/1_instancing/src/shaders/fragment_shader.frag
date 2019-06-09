#version 330 core

in vec3 color2;
out vec4 color3;

void main()
{
	color3 = vec4(color2, 1.0f);
}

