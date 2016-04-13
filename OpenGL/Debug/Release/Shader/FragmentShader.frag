#version 400 core
in vec3 worldNormal;

layout(location = 0) out vec4 outColor;

void main()
{
	//outColor = vec4(0.0, 1.0, 0.0, 1.0);			// green
	outColor = vec4((worldNormal+1)*0.5, 1.0);		// multicolor 

}