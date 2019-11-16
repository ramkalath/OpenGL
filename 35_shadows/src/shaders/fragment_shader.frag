#version 330 core

in vec3 Norm;
in vec3 FragmentPosition;
in vec2 TexCoord;

uniform vec3 LightDirection;
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;
uniform vec3 CameraPosition;

uniform vec3 materialambient;
uniform vec3 materialdiffuse;
uniform vec3 materialspecular;
uniform float materialshininess;

uniform sampler2D my_texture;

out vec4 color;


void main()
{
	// Ambient Light
	vec3 ambient_light = LightAmbient * vec3(texture(my_texture, TexCoord));

	// Diffused Light
	vec3 light_vector = normalize(LightDirection);
	float diff = max(dot(Norm, light_vector), 0.0f);
	vec3 diffuse_light = LightDiffuse * diff * vec3(texture(my_texture, TexCoord));

    // Specular lighting
	vec3 reflected_vector = reflect(light_vector, Norm);  
	vec3 view_vector = normalize(CameraPosition - FragmentPosition);
	float spec = pow(max(dot(view_vector, reflected_vector), 0.0), materialshininess);
	vec3 specular_light = LightSpecular * spec * vec3(texture(my_texture, TexCoord));

	color = vec4((ambient_light + diffuse_light + specular_light), 1.0f);
}
