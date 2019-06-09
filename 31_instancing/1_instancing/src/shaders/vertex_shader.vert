#version 330 core
layout (location=0) in vec2 position; // vertex_position attribute
layout (location=1) in vec3 color1; // vertex_position attribute

out vec3 color2;

uniform vec2 offsets[100];

void main()
{
	vec2 offset = offsets[gl_InstanceID];
	gl_Position = vec4(position + offset, 0.0f, 1.0f);;
	color2 = color1;
}

