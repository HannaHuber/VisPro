#pragma once

#include "glm.hpp"
#include "QuadShader.h"
#include <list>
#include <vector>

class Quad {
public:
	Quad();
	~Quad();
private:
	GLuint vao, vbo;
	static GLfloat vertices[24];
	QuadShader* shader;
	
};

