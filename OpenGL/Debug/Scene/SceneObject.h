#pragma once

// Standard
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

// OpenGL extensions
#include "glew.h"
#include "glfw3.h"
#include <GL/gl.h>
#include "glm.hpp"

class SceneObject {
public:
	SceneObject(glm::mat4& model_matrix);
	virtual ~SceneObject();
	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;

	// Model matrix
	glm::mat4 model_matrix;

	// location
	glm::vec3 location();
	
};