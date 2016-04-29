#pragma once

#include "Shader.h"
#include "PointLight.h"
#include <vector>
#include <memory>

class BlinnPhongShader : public Shader {
public:
	BlinnPhongShader(glm::vec3 amb_col, glm::vec3 diff_col, glm::vec3 spec_col, float shin, float transp);
	~BlinnPhongShader();
	virtual void setLighting(std::vector<std::shared_ptr<PointLight>> *allLights);

private:
	glm::vec3 ambient_color;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	float shininess;
	float transparency;

};