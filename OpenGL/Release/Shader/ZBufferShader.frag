// FRAGMENT SHADER calculate distance to light for each fragment
#version 400 core

in vec4 world_pos;

uniform vec3 light_pos;
uniform vec2 sm_planes;

void main() {
	float dist = length(world_pos.xyz - light_pos);
	dist = dist/sm_planes.y;
	gl_FragDepth = dist*1.02 + 0.003;
	
}