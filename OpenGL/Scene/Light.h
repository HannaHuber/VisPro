#pragma once

#include "SceneObject.h"
#include "glm.hpp"

class Light : public SceneObject {
public:
	Light(glm::mat4& model_matrix);
	virtual ~Light();
	virtual void update(float deltaTime)=0;
	virtual void draw() = 0;
	
private:
	

};