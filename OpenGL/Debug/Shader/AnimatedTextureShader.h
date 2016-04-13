#pragma once
#include "TextureShader.h"
#include <math.h> 
class AnimatedTextureShader : public TextureShader{
public:
	AnimatedTextureShader(const std::string& path, glm::vec3 amb_col, glm::vec3 spec_col, float shin, float framerate);
	~AnimatedTextureShader();
	void bindTexture(int unit);
	private:
		float length; //length of the clip
		float framerate; //fps
		int frames; //amount of frames
		float startTimeVid; 

		std::vector<Texture*> framesOfVid;
};