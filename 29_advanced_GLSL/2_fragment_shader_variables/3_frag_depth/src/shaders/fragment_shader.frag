#version 330 core
#extension GL_ARB_conservative_depth: enable
layout (depth_greater) out float gl_FragDepth;

out vec4 color;

void main()
{
	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	gl_FragDepth = gl_FragCoord.z + 0.02f;
	if(gl_FragCoord.z<0.98f)
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

