#version 400 core
in vec3 position;
in vec3 normal;

out vec3 worldNormal;

uniform mat4 model; // contains position
uniform mat4 view_proj; // contains camera-position etc

void main(){
	gl_Position = view_proj * model * vec4(position, 1);
	worldNormal = (model * vec4(normal,0)).xyz;
}