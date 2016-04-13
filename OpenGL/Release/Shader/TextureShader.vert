#version 400 core
in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 worldNormal;
out vec4 worldPos4D;
out vec3 viewVector;
out vec2 tex_coords;

uniform mat4 model; 
uniform mat4 view_proj; 
uniform vec3 camera_coords;
//uniform mat4 mvpT_from_light;

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

void main(){
	gl_Position = view_proj * model * vec4(position, 1);
	worldPos4D = model * vec4(position, 1);
	worldNormal = normalize(mat3(transpose(inverse(model)))*normalize(normal));
	viewVector =  camera_coords - worldPos4D.xyz;
	tex_coords = uv;
}