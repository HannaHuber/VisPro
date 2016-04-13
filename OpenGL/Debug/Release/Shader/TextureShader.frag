#version 400 core
in vec3 worldNormal;
in vec3 worldPos;
in vec3 viewVector;
in vec2 tex_coords;

uniform mat4 model; 
uniform mat4 view_proj; 
uniform vec3 camera_coords;

uniform int light_count;
uniform vec3 light_location[20]; //Note: not more than 20 lights are allowed
uniform vec3 light_intensity[20];
uniform vec3 light_attenuation[20];
/*


struct Light {
	vec3 location;
	vec3 intensity;
	vec3 attenuation;
};
uniform Light light;*/

uniform sampler2D colorTexture;
struct Material {
	vec3 ambient_color;
	vec3 specular_color;
	float shininess;
};
uniform Material material;

layout(location = 0) out vec4 outColor;

void main()
{
		outColor = vec4(0,0,0,0);
		vec3 diffuse_color = texture(colorTexture,tex_coords).rgb;
		vec3 ambient_color = material.ambient_color;
		vec3 specular_color = material.specular_color;
		float shininess = material.shininess; 
		normalize(viewVector);
		normalize(worldNormal);

		for(int i = 0; i < light_count; i++){
			

			vec3 L = normalize(light_location[i]-worldPos); // Vector from position(fragment) to light source
			vec3 H = normalize(L+viewVector);
		
			float d = length(light_location[i] - worldPos);									// Distance between pos and point light
			float a = light_attenuation[i].x + d*light_attenuation[i].y + d*d*light_attenuation[i].z;// Attenuation
			vec3 I = light_intensity[i] / a;													// Intensity

			float NdotL = max(dot(worldNormal,L),0.0);
			float NdotH = max(dot(worldNormal,H),0.0);
	
			vec3 color = diffuse_color*NdotL + specular_color*NdotL*pow(NdotH,shininess);
			vec3 illumination = I*(color+material.ambient_color); //
		
			outColor = outColor+vec4(illumination, 1.0);		
		}

		if (max(outColor.x, max(outColor.y, outColor.z))>1) {
			outColor = outColor * 1.0f/max(outColor.x, max(outColor.y, outColor.z));
		}

}	