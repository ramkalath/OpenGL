#version 330 core
layout (location = 0) in vec3 position; // vertex position attribute
layout (location = 1) in float scale; // vertex position attribute
layout (location = 2) in vec3 shift; // vertex position attribute

uniform mat4 view;
uniform mat4 projection_perspective;

void main()
{
	mat4 model = mat4(scale, 0, 0, 0,
					  0, scale, 0, 0,
					  0, 0, 1.0f, 0,
					  shift.x, shift.y, shift.z, 1.0f);

	gl_Position = projection_perspective * view * model * vec4(position, 1.0f);
}


// construct a model matrix given scale value and translation; remember column major
