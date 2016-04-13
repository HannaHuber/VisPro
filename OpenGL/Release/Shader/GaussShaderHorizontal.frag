#version 400 core
in vec2 TexCoords;
//layout(location = 0) out vec4 color;
out vec4 color;

uniform sampler2D screenTexture;


void main()
{ 	
	
	
	const float kernel[5] = float[](0.0625, 0.25, 0.375, 0.25, 0.0625);

	vec4 result = vec4(0,0,0,0);

	
	for(int i = 0; i<5; i++){
	//"Sampler types that forbid mipmaps (rectangle, buffer, etc), multisample samplers, and shadow cubemap array samplers cannot be used with texture LOD functions."
	//"The value of offset must be a constant expression"?!
		//color = color+ kernel[i]*textureOffset(screenTexture,TexCoords.st,ivec2(0,i-2)); //"Cubemap, multisample, and buffer samplers are not allowed as types for gsampler." when using textureOffset. Do I have a buffer sampler?
		color = color+ kernel[i]*textureLodOffset(screenTexture,TexCoords.st,9,ivec2(i-2,0));
	}
	//color = texture2D(screenTexture,TexCoords.st);
	
}





/*
void main()
{ 
const float kernel[5] = float[](0.0625, 0.25, 0.375, 0.25, 0.0625);
	//const float kernel[5] = float[](0.2, 0.35, 0.475, 0.35, 0.2);
	//const float kernel[5] = float[](0.2, 0.2, 0.2, 0.2, 0.2);

	color = vec4(0,0,0,0);
	
	vec4 thispixel = texture(screenTexture,TexCoords.st);
	int sameColors = 0;
	int differentColors = 0;
	for(int i = 0; i<5; i++){
		color = color+ kernel[i]*textureOffset(screenTexture,TexCoords.st,ivec2(i-2,0));
			//for(int i = -20; i<20; i++){
				//color = color+ textureOffset(screenTexture,TexCoords.st,ivec2(0,300));
					//	vec4 neighbour = texture(screenTexture,vec2(TexCoords.x,TexCoords.y+i/100));
						//color = color+ neighbour;
						//if(i!=0 && neighbour.r == thispixel.r && neighbour.g == thispixel.g && neighbour.b == thispixel.b){
						//sameColors++;
					//	}
					//	if(!(neighbour.r == thispixel.r && neighbour.g == thispixel.g && neighbour.b == thispixel.b)){
					//	differentColors++;
					//	}
						//color = color+ textureOffset(screenTexture,TexCoords.st,ivec2(0,i));
	//color = texture(screenTexture,TexCoords.st);
	//color = vec4(1,0,0,1);
	}
	//color += kernel[i]*texture(screenTexture, vec2(TexCoords.s,TexCoords.t+(i-2)));
			//if(sameColors>=39){
			//color = vec4(1,0,0,1);
		//	}
		//	if(differentColors>0){
				//		color = vec4(0,1,0,1);
			//}
			
		//	color = texture(screenTexture,vec2(0,0.5));
			
}
*/