#version 400 core
in vec2 TexCoords;
//layout(location = 0) out vec4 color;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{ 	
	
	
	const float kernel[5] = float[](0.0625, 0.25, 0.375, 0.25, 0.0625);

	color = vec4(0,0,0,0);

	for(int i = 0; i<5; i++){
		//color = color+ kernel[i]*textureOffset(screenTexture,TexCoords.st,ivec2(0,i-2));
		color = color+ kernel[i]*textureLodOffset(screenTexture,TexCoords.st,9,ivec2(0,i-2));
	}

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
		color = color+ kernel[i]*textureOffset(screenTexture,TexCoords.st,ivec2(0,i-2));
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
	color = texture(screenTexture,TexCoords.st);
	}

}
*/










	//color += kernel[i]*texture(screenTexture, vec2(TexCoords.s,TexCoords.t+(i-2)));
			//if(sameColors>=39){
			//color = vec4(1,0,0,1);
		//	}
		//	if(differentColors>0){
				//		color = vec4(0,1,0,1);
			//}
			
		//	color = texture(screenTexture,vec2(0,0.5));
			

			/*
float diff = 600;
	
	float kernel[5] = float[](0.0625, 0.25, 0.375, 0.25, 0.0625);
	//float kernel[5] = float[](0.2, 0.35, 0.475, 0.35, 0.2);

    vec2 offsets[5] = vec2[](
		vec2(-2f/diff, 0.0f),vec2(-1f/diff, 0.0f),vec2(0f/diff, 0.0f),vec2(1f/diff, 0.0f),vec2(2f/diff, 0.0f)
	);
	vec3 outcol = vec3(0,0,0);

	for(int i = 0; i<5; i++){
		vec3 otherPixel = vec3(texture(screenTexture, TexCoords.st+offsets[i]));
		//if(otherPixel.r >= 1 || otherPixel.g >= 1 ||otherPixel.b >= 1){
			outcol += otherPixel*kernel[i]; 
		//}
	}
	
	color = vec4(outcol.r, outcol.g, outcol.b, 1);

	*/
