#version 330 core

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

uniform vec3 lamp_pos;
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;
uniform vec3 CameraPosition;


uniform vec3 materialambient;
uniform vec3 materialdiffuse;
uniform vec3 materialspecular;
uniform float materialshininess;

in vec3 Norm;
in vec3 FragmentPosition;
in float Shini;
in vec2 TexCoords;

out vec4 color;

void main()
{
	// Ambient Lighting
	vec3 ambient_light = LightAmbient * vec3(texture(diffuse_texture, TexCoords));

	// Diffused Lighting
	vec3 light_vector = normalize(lamp_pos - FragmentPosition);
	float diff = max(dot(Norm, light_vector), 0.0f);	
	vec3 diffuse_light = LightDiffuse * diff * vec3(texture(diffuse_texture, TexCoords));

    // Specular lighting
    vec3 reflected_vector = reflect(light_vector, Norm);  
    vec3 view_vector = normalize(CameraPosition - FragmentPosition);
    float spec = pow(max(dot(view_vector, reflected_vector), 0.0), Shini);
    vec3 specular_light = LightSpecular * spec * vec3(texture(specular_texture, TexCoords));

	color = vec4((ambient_light + diffuse_light + specular_light), 1.0f);
}
