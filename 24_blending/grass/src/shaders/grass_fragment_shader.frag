#version 330 core

uniform sampler2D this_texture;

out vec4 color;
in vec2 texCoords;

void main()
{
	vec4 texcolor = texture(this_texture, texCoords);
	if(texcolor.a < 0.1)
		discard;
	color = texcolor;
}

