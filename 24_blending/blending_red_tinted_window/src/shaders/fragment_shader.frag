#version 330 core

uniform sampler2D this_texture;

out vec4 color;
in vec2 texCoords;

void main()
{
	color = texture(this_texture, texCoords);
	//color = vec4(vec3(gl_FragCoord.z), 1.0f);
}

