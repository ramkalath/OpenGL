#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
	vec3 color;
} gs_in;

void main()
{
	gl_Position = gl_in[0].gl_Position + vec4(-0.3, -0.3, 0.0, 0.0); //bottom left
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.3, -0.3, 0.0, 0.0); //bottom right
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(-0.3, 0.3, 0.0, 0.0); //top left
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.3, 0.3, 0.0, 0.0); //top right
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.5, 0.0, 0.0); //top center
	EmitVertex();

	EndPrimitive();
}
