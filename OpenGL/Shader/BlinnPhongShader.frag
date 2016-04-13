#version 400 core
in vec3 worldNormal;
in vec4 worldPos4D;
in vec3 viewVector;
//in vec4 sm_coords;

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

struct Material {
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	float shininess;
	float transparency;
};
uniform Material material;

// uniform sampler2D shadow_map; // BASIC shadow mapping
uniform samplerCubeShadow shadow_map;  // CUBE shadow mapping
uniform int sm_light_id;
uniform vec3 sm_light_pos;
uniform vec2 planes;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 worldPos = worldPos4D.xyz;

	outColor = vec4(0,0,0,0);
	vec3 ambient_color = material.ambient_color;
	vec3 diffuse_color = material.diffuse_color;
	vec3 specular_color = material.specular_color;
	float shininess = material.shininess;
	normalize(viewVector);
	normalize(worldNormal);	

	// Shadow mapping scaling factor
	float is_lit = 1.0;
	float map_dist = 0.0;

	for(int i = 0; i < light_count; i++){

		vec3 light_dir = light_location[i]-worldPos;
		vec3 L = normalize(light_dir); // Vector from position(fragment) to light source
		vec3 H = normalize(L+viewVector);
		
		float d = length(light_dir);									// Distance between pos and point light
		float a = light_attenuation[i].x + d*light_attenuation[i].y + d*d*light_attenuation[i].z;// Attenuation
		vec3 I = light_intensity[i] / a;													// Intensity

		float NdotL = max(dot(worldNormal,L),0.0);
		float NdotH = max(dot(worldNormal,H),0.0);
		if(material.transparency < 1){
		NdotL = abs(dot(worldNormal,L));
		NdotH = abs(dot(worldNormal,H));
		}
		vec3 color = diffuse_color*NdotL + specular_color*NdotL*pow(NdotH,shininess);

		// Perform CUBE shadow mapping for one light source
		if (i==sm_light_id) {
			float d_norm = (d-planes.x)/(planes.y - planes.x);
			d_norm = clamp(d_norm, 0, 1);
			normalize(light_dir);
			 is_lit = texture(shadow_map, vec4(-light_dir,d/planes.y)) ;
			 //is_lit = (d_norm>map_dist+0.05) ? 0.5 : 1.0;
			 color *= is_lit;
		}
		vec3 illumination = I*(color) + ambient_color;
				
		outColor = outColor+vec4(illumination, 1);
	}

	// DEBUG: Show SM only
		//vec3 light_dir = worldPos - sm_light_pos;
		//map_dist = texture(shadow_map, light_dir).r ;
		//float d = distance(worldPos, sm_light_pos);
		//is_lit = (d>map_dist+0.15) ? 0.5 : 1.0;
		//vec3 illumination = vec3(1.0) * is_lit;
		//outColor = vec4(vec3(d/far_plane), 1.0);
	
	// Scale out_color to [0,1]
	if (max(outColor.x, max(outColor.y, outColor.z))>1) {
		outColor = outColor * 1.0f/max(outColor.x, max(outColor.y, outColor.z));
	}
	outColor = vec4(outColor.xyz,material.transparency);
		
}