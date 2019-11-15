#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Norm;
out vec3 FragmentPosition;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	FragmentPosition = vec3(model * vec4(position, 1.0f));
	Norm = normalize(vec3(model * vec4(normal, 1.0f)));

	TexCoord = texcoord;
}
