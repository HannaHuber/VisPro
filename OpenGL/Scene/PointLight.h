#pragma once

#include "Light.h"
#include "glm.hpp"

class PointLight : public Light {
public:
	PointLight(glm::mat4& model_matrix, glm::vec3 intensity, float catt, float latt, float qatt);
	virtual ~PointLight();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec3 intensity;
	float const_attenuation;
	float lin_attenuation;
	float quad_attenuation;
	
};