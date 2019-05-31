#version 330 core

in vec3 color2;
out vec3 color3;

void main()
{
	color3 = vec3(color2, 1.0f);
}

