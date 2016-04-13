#include "BlinnPhongShader.h"
#include "gtc/type_ptr.hpp"


using namespace glm;

BlinnPhongShader::BlinnPhongShader(vec3 amb_col, vec3 diff_col, glm::vec3 spec_col, float shin)
: Shader("./Shader/BlinnPhongShader.vert", "./Shader/BlinnPhongShader.frag")
, ambient_color(amb_col)
, diffuse_color(diff_col)
, specular_color(spec_col)
, shininess(shin)
{
	useShader();

	// Set material specific uniforms
	auto amb_col_location = glGetUniformLocation(programHandle, "material.ambient_color");
	auto diff_col_location = glGetUniformLocation(programHandle, "material.diffuse_color");
	auto spec_col_location = glGetUniformLocation(programHandle, "material.specular_color");
	auto shin_location = glGetUniformLocation(programHandle, "material.shininess");
	glUniform3fv(amb_col_location, 1, glm::value_ptr(ambient_color));
	glUniform3fv(diff_col_location, 1, glm::value_ptr(diffuse_color));
	glUniform3fv(spec_col_location, 1, glm::value_ptr(specular_color));
	glUniform1f(shin_location, shininess);

	glUseProgram(0);
}

BlinnPhongShader::~BlinnPhongShader() {

}

void BlinnPhongShader::setLighting(std::vector<std::shared_ptr<PointLight>> *allLights) {
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
	glUniform1i(light_count_location, allLights->size()); //works without value_ptr?
	glUniform3fv(light_loc_location, 20, glm::value_ptr(location[0]));
	glUniform3fv(light_int_location, 20, glm::value_ptr(intensity[0]));
	glUniform3fv(light_att_location, 20, glm::value_ptr(att[0]));

	glUseProgram(0);
}

