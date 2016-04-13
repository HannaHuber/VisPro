#pragma once
#include "Shader.h"
#include <vector>
#include <memory>
class NormalShader : public Shader {
public: 
	NormalShader();
	virtual void setLighting(std::vector<std::shared_ptr<PointLight>> *allLights);
		

};