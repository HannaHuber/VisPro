#version 400 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 qCoords[8];

uniform mat4 model;
uniform mat4 view_proj; 

uniform int step;

void main()
{
    gl_Position = view_proj * model * vec4(position.x, position.y, 0, 1); 

	qCoords[0] = vec2(gl_Position.x + step, gl_Position.y - step);
	qCoords[1] = vec2(gl_Position.x + step, gl_Position.y );
	qCoords[2] = vec2(gl_Position.x + step, gl_Position.y + step);

	qCoords[3] = vec2(gl_Position.x, gl_Position.y - step);
	qCoords[4] = vec2(gl_Position.x, gl_Position.y + step);

	qCoords[5] = vec2(gl_Position.x - step, gl_Position.y - step);
	qCoords[6] = vec2(gl_Position.x - step, gl_Position.y );
	qCoords[7] = vec2(gl_Position.x - step, gl_Position.y + step);

} 