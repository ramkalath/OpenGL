#version 330 core

// Input vertex data, different for all exectutions of this shader.
layout(location=0) in vec3 vertexPosition_modelspace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1);
}
