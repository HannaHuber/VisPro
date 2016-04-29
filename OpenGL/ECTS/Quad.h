#pragma once

#include "glm.hpp"
#include <list>
#include <vector>

class Quad {
public:
	Quad();
	~Quad();
	void useShader();
	void bindVAO();
private:
	GLuint vao, vbo;
	static GLfloat vertices[24];
	
};

