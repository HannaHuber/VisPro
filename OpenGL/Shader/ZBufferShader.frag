// FRAGMENT SHADER calculate distance to light for each fragment
#version 400 core

layout(location = 0) out vec4 outColor;


void main() {
	// Screen space coordinates
	outColor.rg = gl_FragCoord.xy;

	// Depth values
	outColor.b = gl_FragCoord.z;
	
}