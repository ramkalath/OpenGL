#version 330 core

uniform sampler2D this_texture;

out vec4 color;
in vec2 texCoords;

void main()
{
	color = texture(this_texture, texCoords);
}

