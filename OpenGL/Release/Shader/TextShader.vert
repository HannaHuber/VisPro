#version 400 core
layout (location = 0) in vec4 positionAndTexCoords; //trick: combine vertex position and texture coords into 1 vector for easier passing
out vec2 texCoords;

uniform mat4 proj_mat;

void main()
{
    gl_Position = proj_mat * vec4(positionAndTexCoords.x, positionAndTexCoords.y,0,1); 
   //  gl_Position = vec4(positionAndTexCoords.x, positionAndTexCoords.y,0,1); 
   
	texCoords = positionAndTexCoords.zw;
}  