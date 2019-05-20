#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D this_texture;

const float offset = 1.0/300;

void main()
{
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),

		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(-offset, 0.0f),

		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	float kernel[9] = float[](
			1.0/16, 2.0/16, 1.0/16,
			2.0/16, 4.0/16, 2.0/16,
			1.0/16, 2.0/16, 1.0/16);

	vec3 sampleTex[9];
	for(int i=0; i<9; i++)
		sampleTex[i] = vec3(texture(this_texture, texCoords.st + offsets[i]));

	vec3 col;
	for(int i=0; i<9; i++)
		col += sampleTex[i] * kernel[i];

	color = vec4(col, 1.0);
}
