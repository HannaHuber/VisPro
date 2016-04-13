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

#define PLANE_VERTEX_COUNT 4 // #vertices x #attributes (normals, etc) per vertex
#define PLANE_INDEX_COUNT 6 // #triangles x3

class Plane : public SceneObject {
public:
	Plane::Plane(glm::mat4& model_matrix, Shader* shader);
	virtual ~Plane();
	virtual void update(float deltaTime);
	virtual void draw();

	// Model matrix
	glm::mat4 model_matrix;

private:

	// Vertex data
	static const float vertex_positions[3 * PLANE_VERTEX_COUNT];		// Vertex positions
	static const float vertex_normals[3 * PLANE_VERTEX_COUNT];		// Vertex normals
	static const float vertex_uvs[2 * PLANE_VERTEX_COUNT];		// Vertex uv coords
	static const unsigned int vertex_indices[PLANE_INDEX_COUNT];	// Vertex indices
	
	// Vertex buffer objects
	GLuint pos_vbo, normal_vbo, uv_vbo, index_vbo;

	// Vertex array object
	GLuint vao;

	// Shader
	Shader* shader;





};