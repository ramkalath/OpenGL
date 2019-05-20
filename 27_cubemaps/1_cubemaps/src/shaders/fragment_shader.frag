#version 330 core

uniform sampler2D mytexture;

out vec4 color;
in vec2 texCoords;

void main()
{
	color = texture(mytexture, texCoords);
}

