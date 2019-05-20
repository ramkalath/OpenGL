#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D this_texture;

void main()
{
	color = texture(this_texture, texCoords);
	float average = (color.r+color.b+color.g)/3;
	color = vec4(average, average, average, 1.0);
}
