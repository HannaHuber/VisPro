#version 400 core
in vec3 worldNormal;
in vec4 worldPos4D;
in vec3 viewVector;
in vec2 tex_coords;
//in vec4 sm_coords;

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

// uniform sampler2D shadow_map; // BASIC shadow mapping
uniform samplerCube shadow_map;  // CUBE shadow mapping
uniform int sm_light_id;
uniform vec3 sm_light_pos;
uniform float far_plane;

layout(location = 0) out vec4 outColor;

void main()
{		
		vec3 worldPos = worldPos4D.xyz;

		// Apply illumination model
		outColor = vec4(0,0,0,0);
		vec3 diffuse_color = texture(colorTexture,tex_coords).rgb;
		vec3 ambient_color = material.ambient_color;
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
	
			vec3 color = diffuse_color*NdotL + specular_color*NdotL*pow(NdotH,shininess);
			vec3 illumination = I*(color+material.ambient_color); //
		
			// Perform CUBE shadow mapping for one light source
			if (i==sm_light_id) {
			 map_dist = texture(shadow_map, -light_dir).r * far_plane; //
			 is_lit = (d>map_dist+0.15) ? 0.5 : 1.0;
			 illumination *= is_lit;
			}

			outColor = outColor+vec4(illumination, 1.0);
		}

		// DEBUG: Show SM only
		//vec3 light_dir = worldPos - sm_light_pos;
		//map_dist = texture(shadow_map, light_dir).r ;
		//float d = distance(worldPos, sm_light_pos);
		//is_lit = (d>map_dist+0.15) ? 0.5 : 1.0;
		//vec3 illumination = vec3(1.0) * is_lit;
		//outColor = vec4(vec3(d/far_plane), 1.0);

		// Scale outColor to [0,1]
		if (max(outColor.x, max(outColor.y, outColor.z))>1) {
			outColor = outColor * 1.0f/max(outColor.x, max(outColor.y, outColor.z));
		}

		// Perform BASIC shadow mapping
		//vec2 sm_uvs = sm_coords.xy / sm_coords.w; 
		//float depth_p = sm_coords.z / sm_coords.w; 
		//vec2 sm_uvs = (sm_coords.xy / sm_coords.w) *0.5 +0.5; 
		//float depth_p = (sm_coords.z / sm_coords.w)*0.5 +0.5; 
		//float depth_c = texture(shadow_map, sm_uvs).r; 
		//float is_lit = (depth_c < depth_p) ? 0.2 : 1.0;
		//outColor *= is_lit;
		// DEBUG
		//outColor = vec4(texture(shadow_map, worldPos- light_location[light_id]).r, 0.0,0.0,1.0);		
		//outColor = vec4(vec3(depth_c),1.0);


}	