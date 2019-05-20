#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D this_texture;

void main()
{
	color = texture(this_texture, texCoords);
}
