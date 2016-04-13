// FRAGMENT SHADER calculate distance to light for each fragment
#version 400 core

in vec4 world_pos;

uniform vec3 light_pos;
uniform float sm_far_plane;

void main() {
	float dist = length(world_pos.xyz - light_pos);
	gl_FragDepth = dist/sm_far_plane;
	
}