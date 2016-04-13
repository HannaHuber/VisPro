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

#include "Shader.h"
#include "SceneObject.h"

#define VERTEX_COUNT 3 // #vertices x #attributes (normals, etc) per vertex
#define INDEX_COUNT 3 // #faces x3

class Triangle : public SceneObject {
public:
	Triangle::Triangle(glm::mat4& model_matrix, Shader* shader);
	virtual ~Triangle();
	virtual void update(float deltaTime);
	virtual void draw();

	// Model matrix
	glm::mat4 model_matrix;

private:

	// Vertex data
	static const float vertex_positions[3 * VERTEX_COUNT];		// Vertex positions
	static const float vertex_normals[3 * VERTEX_COUNT];		// Vertex normals
	static const float vertex_uvs[2 * VERTEX_COUNT];		// Vertex uv coords
	static const unsigned int vertex_indices[INDEX_COUNT];	// Vertex indices
	
	// Vertex buffer objects
	GLuint pos_vbo, normal_vbo, uv_vbo, index_vbo;

	// Vertex array object
	GLuint vao;

	// Shader
	Shader* shader;





};