#version 400 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D fontTexture;
uniform vec4 fontColor;

void main()
{ 
	color = vec4(1,1,1,texture(fontTexture, texCoords).r) * fontColor; 
	//color = vec4(1,1,1,texture(fontTexture, texCoords)); 
	//color = fontColor;
	//color = vec4(0,texCoords.x,texCoords.y,1);
	//color = vec4(0,1,0,1);
	//for(int i = 0; i<0; i++){
	//	i++;
	//}
}