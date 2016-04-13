#include "Triangle.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

using namespace std;

Triangle::Triangle(glm::mat4& model_matrix, Shader* shader) 
: SceneObject(model_matrix)
, shader(shader) {

	// Init position VBO						
	glGenBuffers(1, &pos_vbo);																				// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);																	// Bind VBO
	glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * 3 * sizeof(float), vertex_positions, GL_STATIC_DRAW);			// Copy vertex data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);																		// Unbind VBO

	// Init normal VBO						
	glGenBuffers(1, &normal_vbo);																			// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);														// Bind VBO
	glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * 3 * sizeof(float), vertex_normals, GL_STATIC_DRAW);    // Copy normal data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);																// Unbind VBO

	// Init uv VBO						
	glGenBuffers(1, &uv_vbo);																			// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);														// Bind VBO
	glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * 2 * sizeof(float), vertex_uvs, GL_STATIC_DRAW);// Copy uv data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);															// Unbind VBO

	// Init index VBO						
	glGenBuffers(1, &index_vbo);																			// Create VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);														// Bind VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_COUNT*sizeof(unsigned int), vertex_indices, GL_STATIC_DRAW);// Copy index data into VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);																// Unbind VBO

	// Link shader attributes and vertex data
	glGenVertexArrays(1, &vao);												 // Create VAO
	glBindVertexArray(vao);													 // Bind VAO

	// Position buffer
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);									 // Bind VBO
	GLint posAttrib = glGetAttribLocation(shader->programHandle, "position"); // Get attribute location
	glEnableVertexAttribArray(posAttrib);									  // Enable attribute array																	  // Create VAO (link attributes and VBOs)
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);			  // Specify attribute array

	// Normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);									 // Bind VBO
	GLint normAttrib = glGetAttribLocation(shader->programHandle, "normal"); // Get attribute location
	glEnableVertexAttribArray(normAttrib);									  // Enable attribute array																	  // Create VAO (link attributes and VBOs)
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);			  // Specify attribute array

	// UV coords buffer
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);									 // Bind VBO
	GLint uvAttrib = glGetAttribLocation(shader->programHandle, "uv"); // Get attribute location
	glEnableVertexAttribArray(uvAttrib);									  // Enable attribute array																	  // Create VAO (link attributes and VBOs)
	glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);			  // Specify attribute array

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
	
	// Unbind 
	glBindVertexArray(0);													  // Unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);									   	  // Unbind VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);								  // Unbind index VBO
	
}

Triangle::~Triangle() {
	glDeleteBuffers(1, &pos_vbo);
	glDeleteBuffers(1, &normal_vbo);
	glDeleteBuffers(1, &uv_vbo);
	glDeleteBuffers(1, &index_vbo);
	glDeleteVertexArrays(1, &vao);
} 

void Triangle::draw() {
	shader->useShader();// Use Shader program	

	// Get model matrix
	auto model_location = glGetUniformLocation(shader->programHandle, "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

	// Draw
	glBindVertexArray(vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, INDEX_COUNT, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void Triangle::update(float deltaTime) {
	model_matrix *= glm::rotate(glm::mat4(1.0), deltaTime*1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

const float Triangle::vertex_positions[3 * VERTEX_COUNT] = {
	 // front
	 -0.5f, 0.5f, 0.5f,		//0 
	 0.5f, 0.5f, 0.5f,			//1 
	 0.0f, 5.5f, 0.5f			//2
 
};

const float Triangle::vertex_normals[3 * VERTEX_COUNT] = {
	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f

 };

const float Triangle::vertex_uvs[2 * VERTEX_COUNT] = {
	 0.0f, 0.0f,//0
	 0.5f, 0.0f,//1
	 0.25f, 0.5f//2

	 
 };

const unsigned int Triangle::vertex_indices[INDEX_COUNT] = {
	0,1,2
};

