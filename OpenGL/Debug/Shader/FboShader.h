#pragma once
#include "Shader.h"
#include "../ECTS/Texture.h"
#include "PointLight.h"
#include <vector>
#include <memory>

class FboShader : public Shader {
public:
	FboShader();
	~FboShader();
	virtual void bindTexture(int unit);
	virtual void setLighting(std::vector<std::shared_ptr<PointLight>> *allLights);
};