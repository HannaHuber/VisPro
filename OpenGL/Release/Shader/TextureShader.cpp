#include "TextureShader.h"
#include "gtc/type_ptr.hpp"

using namespace glm;

TextureShader::TextureShader(const std::string& path, vec3 amb_col, vec3 spec_col, float shin)
: Shader("./Shader/TextureShader.vert", "./Shader/TextureShader.frag")
, texture(new Texture(path))
{
	useShader();

	// Set material specific uniforms
	auto amb_col_location = glGetUniformLocation(programHandle, "material.ambient_color");
	auto spec_col_location = glGetUniformLocation(programHandle, "material.specular_color");
	auto shin_location = glGetUniformLocation(programHandle, "material.shininess");
	glUniform3fv(amb_col_location, 1, glm::value_ptr(amb_col));
	glUniform3fv(spec_col_location, 1, glm::value_ptr(spec_col));
	glUniform1f(shin_location, shin);

	glUseProgram(0);

}

TextureShader::~TextureShader() {
	delete texture; texture = nullptr;
}

void TextureShader::bindTexture(int unit) {
	texture->bind(unit);
	if (useMipMapping == 0){ //use no mipmapping
		if (useLinearTexSampling){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
	}
	else if (useMipMapping == 1){ //use nearest neighbour mipmapping
		if (useLinearTexSampling){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		}
		else{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
	}
	else{
		if (useLinearTexSampling){ //use linear sampling mipmapping
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		}
	}
	auto tex_location = glGetUniformLocation(programHandle, "colorTexture");
	glUniform1i(tex_location, unit);
}

void TextureShader::setLighting(std::vector<std::shared_ptr<PointLight>> *allLights, int sm_light_id) {
	useShader(); 
	
	glm::vec3 att[20];
	glm::vec3 intensity[20];
	glm::vec3 location[20];

	for (int i = 0; i < allLights->size(); i++){
		att[i] = vec3(allLights->at(i)->const_attenuation, allLights->at(i)->lin_attenuation, allLights->at(i)->quad_attenuation);
		location[i] = allLights->at(i)->location();
		intensity[i] = allLights->at(i)->intensity;
	}
	for (int i = allLights->size(); i < 20; i++){
		att[i] = vec3(0, 0, 0);
		location[i] = vec3(0, 0, 0);
		intensity[i] = vec3(0, 0, 0);
	}

	auto light_count_location = glGetUniformLocation(programHandle, "light_count");
	auto light_loc_location = glGetUniformLocation(programHandle, "light_location");
	auto light_int_location = glGetUniformLocation(programHandle, "light_intensity");
	auto light_att_location = glGetUniformLocation(programHandle, "light_attenuation");
	glUniform1i(light_count_location, allLights->size()); 
	glUniform3fv(light_loc_location, 20, glm::value_ptr(location[0]));
	glUniform3fv(light_int_location, 20, glm::value_ptr(intensity[0]));
	glUniform3fv(light_att_location, 20, glm::value_ptr(att[0]));

	// Shadow map uniforms
	auto id_loc = glGetUniformLocation(programHandle, "sm_light_id");
	auto pos_loc = glGetUniformLocation(programHandle, "sm_light_pos");
	glUniform1i(id_loc, sm_light_id);
	glUniform3fv(pos_loc, 1, glm::value_ptr(location[sm_light_id]));

	glUseProgram(0);
}