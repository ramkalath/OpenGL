#version 330 core
layout (location = 0) in vec3 position; // vertex position attribute
layout (location = 1) in float size; // vertex position attribute
layout (location = 2) in vec3 offset; // vertex position attribute

uniform mat4 view;
uniform mat4 projection_perspective;

void main()
{
	//mat4 model = {scale, 0, 0, position.x+offset.x,
				  //0, scale, 0, position.y+offset.y, 
				  //0, 0, scale, position.z+offset.z, 
				  //0, 0, 0, 1};

	mat4 model = {scale, 0, 0, 0,
				  0, scale, 0, 0,
				  0, 0, scale, 0,
				  position.x+offset.x, position.y+offset.y, position.z+offset.z, 1};

	gl_Position = projection_perspective * view * model * vec4(position, 1.0f);
}


// construct a model matrix given scale value and translation; remember column major
