#pragma once

#include "Shader.h"

class QuadShader : public Shader {
public:
	QuadShader(float z_near, float z_far, float angle);
	~QuadShader();
};