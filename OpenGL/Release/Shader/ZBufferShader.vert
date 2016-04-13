// VERTEX SHADER transform vertices to world space
#version 400 core

in vec3 position; 
uniform mat4 model;

void main () {
	gl_Position = model* vec4(position, 1);	
}