#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;
layout (location = 3) in vec3 mat_ambient;
layout (location = 4) in vec3 mat_diffuse;
layout (location = 5) in vec3 mat_specular;
layout (location = 6) in float shininess;

// Transformation Matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Position;
out vec3 Normal;

void main()
{
    gl_Position = projection * view *  model * vec4(position, 1.0f);
	Position = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
}
