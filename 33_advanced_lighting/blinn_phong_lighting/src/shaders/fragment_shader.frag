#version 330 core
out vec4 color;

// in's from vertex shader
in vec3 fragment_position;  
in vec3 norm;  
in vec3 diffuse_light;
  
// lighting uniforms
uniform vec3 lamp_pos; 
uniform vec3 camera_pos;
uniform vec3 light_color;
uniform vec3 plate_color;

// ambient lighting - calculated in the main program
uniform vec3 ambient_light;

void main()
{
	/*
	 * TODO(ram): something is screwing up here while calculating specular reflections. Go one layer deeper and decipher what is screwing up
	 */

    // Specular lighting
	float specular_strength = 1.0f;
	vec3 light_vector = normalize(lamp_pos - fragment_position);
	vec3 view_vector = normalize(camera_pos - fragment_position);
	vec3 halfway_vector = normalize(light_vector+view_vector);
	float specular_value = specular_strength * pow(max(dot(norm, halfway_vector), 0.0), 1); // the shininess property is made very poor
	vec3 specular_light = specular_value * light_color;  
		
	color = vec4((ambient_light + specular_light) * plate_color, 1.0f); // diffuse light is intentionally removed to make the effect of specular light clear
} 
