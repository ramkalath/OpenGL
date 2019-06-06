#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_OUT {
	vec3 norm;
	vec3 fragment_position;
	float shini;
	vec2 texCoords;
} gs_in[];

out vec3 Norm;
out vec3 FragmentPosition;
out float Shini;
out vec2 TexCoords;

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 2.0f;
	vec3 direction = normal * ((sin(time) + 1.0f)/2.0f) * magnitude;
	return position + vec4(direction, 0.0f);
}

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

void main()
{
	vec3 normal  = GetNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	Norm = gs_in[0].norm;
	FragmentPosition = gs_in[0].fragment_position;
	Shini = gs_in[0].shini;
	TexCoords = gs_in[0].texCoords;
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, normal);
	Norm = gs_in[1].norm;
	FragmentPosition = gs_in[1].fragment_position;
	Shini = gs_in[1].shini;
	TexCoords = gs_in[1].texCoords;
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, normal);
	Norm = gs_in[2].norm;
	FragmentPosition = gs_in[2].fragment_position;
	Shini = gs_in[2].shini;
	TexCoords = gs_in[2].texCoords;
	EmitVertex();

	EndPrimitive();
}
