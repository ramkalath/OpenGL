#version 330 core

in vec3 fColor;
out vec4 color;

void main()
{
	color = vec4(fColor, 1.0f);
	//color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

