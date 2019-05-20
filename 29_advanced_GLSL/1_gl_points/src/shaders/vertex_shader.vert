#version 330 core
layout (location = 0) in vec3 position; // vertex_position attribute

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection_perspective;

out vec4 color;
void main()
{
	gl_Position = projection_perspective * view * model * vec4(position, 1.0);
	gl_PointSize = 2.0f;
	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	if(gl_VertexID>3)
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);

}

