#pragma once
#include "Shader.h"
#include "../ECTS/Texture.h"
#include "PointLight.h"
#include <vector>
#include <memory>
#include "../ECTS/UserInput.h"

class TextureShader : public Shader {
public:
	TextureShader(const std::string& path, glm::vec3 amb_col, glm::vec3 spec_col, float shin);
	~TextureShader();
	virtual void bindTexture(int unit);
	virtual void setLighting(std::vector<std::shared_ptr<PointLight>> *allLights, int sm_light_id);
protected:
	Texture* texture;
};