#version 330 core

// Output data
layout(location=0) out float fragmentdepth;

out vec4 color;

void main()
{
	color = vec4(vec3(gl_FragCoord.z), 1.0f);
}
