#version 330 core

//uniform vec3 LightPosition;
uniform vec3 LightDirection;
uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;
uniform vec3 CameraPosition;
//uniform vec3 MatDiff_;


uniform vec3 materialambient;
uniform vec3 materialdiffuse;
uniform vec3 materialspecular;
uniform float materialshininess;

in vec3 Norm;
in vec3 FragmentPosition;
in vec3 MatAmbi;
in vec3 MatDiff;
in vec3 MatSpec;
in float Shini;

out vec4 color;

void main()
{
	// Ambient Lighting
	vec3 ambient_light = LightAmbient * MatAmbi;

	// Diffused Lighting
	vec3 light_vector = normalize(-LightDirection);
	float diff = max(dot(Norm, light_vector), 0.0f);	
	vec3 diffuse_light = LightDiffuse * diff * MatDiff;

    // Specular lighting
    vec3 reflected_vector = reflect(light_vector, Norm);  
    vec3 view_vector = normalize(CameraPosition - FragmentPosition);
    float spec = pow(max(dot(view_vector, reflected_vector), 0.0), Shini);
    vec3 specular_light = LightSpecular * spec * MatSpec;

	color = vec4((ambient_light + diffuse_light), 1.0f);
}
