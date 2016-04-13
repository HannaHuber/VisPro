#pragma once
#include "Shader.h"

/* This class is used for the bloom effect. Because of the VBO we need a plane 
that is filling the screen. On this plane we draw the whole scene as a texture*/
class ScreenFillingQuad{
public:
	ScreenFillingQuad();
	
	// Vertex array object
	GLuint vao;

private:
const static int vertexCount = 4;
const static int indexCount = 6; // #triangles x3
	// Vertex data
	static const float vertex_positions[2 * vertexCount];		// Vertex positions
	//static const float vertex_normals[2 * vertexCount];		// Vertex normals
	//static const float vertex_uvs[2 * vertexCount];		// Vertex uv coords
	static const unsigned int vertex_indices[indexCount];	// Vertex indices

	// Vertex buffer objects
	GLuint pos_vbo, normal_vbo, uv_vbo, index_vbo;

	

	// Shader
	Shader* shader;


	// Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	static const GLfloat quadVertices[24];

};