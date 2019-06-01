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

// Outs
out vec3 FragmentPosition;
out vec3 Norm;

out float Shini;

out VS_OUT {
	vec3 norm;
	vec3 fragment_position;
	float shini;
	vec2 texCoords;
} vs_out;

void main()
{
    gl_Position = projection * view *  model * vec4(position, 1.0f);

	FragmentPosition = vec3(model * vec4(position, 1.0f));
	Norm = normalize(vec3(transpose(inverse(model)) * vec4(normal, 1.0f)));

	vs_out.norm = Norm;
	vs_out.fragment_position = FragmentPosition;
	vs_out.shini = shininess;
	vs_out.texCoords = tex_coords;
}
