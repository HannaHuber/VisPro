#include "PointLight.h"

using namespace glm;

PointLight::PointLight(mat4& model_matrix, glm::vec3 intensity, float catt, float latt, float qatt)
: Light(model_matrix)
, intensity(intensity)
, const_attenuation(catt)
, lin_attenuation(latt)
, quad_attenuation(qatt)
{

}

PointLight::~PointLight() {

}

void PointLight::update(float deltaTime) {

}

void PointLight::draw() {

}
