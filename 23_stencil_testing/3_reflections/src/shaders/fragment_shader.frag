#version 330 core

uniform sampler2D this_texture;
uniform vec3 darken;

out vec4 color;
in vec2 texCoords;

void main()
{
	color = texture(this_texture, texCoords) * vec4(darken, 1.0);
	//color = vec4(vec3(gl_FragCoord.z), 1.0f);
}

