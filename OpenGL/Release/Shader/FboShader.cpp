#include "FboShader.h"
#include "gtc/type_ptr.hpp"

using namespace glm;

FboShader::FboShader()
: Shader("./Shader/FboShader.vert", "./Shader/FboShader.frag")
{
	useShader();


	glUseProgram(0);

}

FboShader::~FboShader() {
}


void FboShader::bindTexture(int unit) {
	//texture->bind(unit);
	//auto tex_location = glGetUniformLocation(programHandle, "colorTexture");
	//glUniform1i(tex_location, unit);
}

void FboShader::setLighting(std::vector<std::shared_ptr<PointLight>> *allLights) {
	/*glm::vec3 att[20];
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
	*/
	}