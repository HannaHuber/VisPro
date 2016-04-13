#include "AnimatedTextureShader.h"
#include "tinydir.h"

using namespace glm;

AnimatedTextureShader::AnimatedTextureShader(const std::string& path, glm::vec3 amb_col, glm::vec3 spec_col, float shin, float framerate)
:TextureShader(path, amb_col, spec_col, shin)
,framerate(framerate){
	startTimeVid = 0;

	frames = 0;
	
	/*
	//testing:
	framesOfVid.push_back(new Texture("textures/fh-green-plain.png"));
	framesOfVid.push_back(new Texture("textures/fh-red-plain.png"));
	framesOfVid.push_back(new Texture("textures/fh-yellow-plain.png"));
	frames = 3;
	*/

	// textures/blabla.jpg --> textures/blabla folder
	int dotPosition = 0;
	for (int i = 0; i < path.length(); i++){
		if(path.at(i) == *"."){
			dotPosition = i;
		}
	}
	std::string subpath = path.substr(0, dotPosition)+"/";
	std::cout << " Truncated path to file " << path << " to path to folder " << subpath<<std::endl;
	tinydir_dir dir;
	tinydir_open(&dir, subpath.c_str());

	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		if (std::string(file.extension) == "jpg" || std::string(file.extension) == "png"){
		//	std::cout << "Importing animated texture: " << subpath + file.name << std::endl;
			framesOfVid.push_back(new Texture(subpath + file.name));
			frames++;
		}
		else{
			//std::cout << "Skip importing of \"" << file.extension << "\"-file..."<<std::endl;
		}
		tinydir_next(&dir);
	}

	tinydir_close(&dir);
	
	length = ((float)frames)/framerate;
}
void AnimatedTextureShader::bindTexture(int unit) {
	//std::cout << "BINDED TEXTURE IN ANIMATEDTEXTURESHADER" << std::endl << std::endl;

	if (startTimeVid == 0.0){ // video is starting now
		startTimeVid = (float)glfwGetTime();
	}
	float passedTime = fmod((((float)glfwGetTime()) - startTimeVid), length); //fmod == float modulu
	int frameIndex = floor((passedTime / length)*frames);

	framesOfVid[frameIndex]->bind(unit);
	auto tex_location = glGetUniformLocation(programHandle, "colorTexture");
	glUniform1i(tex_location, unit);
}