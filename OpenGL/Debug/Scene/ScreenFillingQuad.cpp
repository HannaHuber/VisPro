#include "ScreenFillingQuad.h"

using namespace std;


ScreenFillingQuad::ScreenFillingQuad(){
	
	GLuint quadVBO; //http://learnopengl.com/code_viewer.php?code=advanced/framebuffers_screen_texture
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	/*
	// Init position VBO						
	glGenBuffers(1, &pos_vbo);																				// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);																	// Bind VBO
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), vertex_positions, GL_STATIC_DRAW);			// Copy vertex data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);																		// Unbind VBO
	
	// Init normal VBO						
	glGenBuffers(1, &normal_vbo);																			// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);														// Bind VBO
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertex_normals, GL_STATIC_DRAW);    // Copy normal data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);																// Unbind VBO

	// Init uv VBO						
	glGenBuffers(1, &uv_vbo);																			// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);														// Bind VBO
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), vertex_uvs, GL_STATIC_DRAW);// Copy uv data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);															// Unbind VBO
	
	// Init index VBO						
	glGenBuffers(1, &index_vbo);																			// Create VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);														// Bind VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(unsigned int), vertex_indices, GL_STATIC_DRAW);// Copy index data into VBO
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

	*/
}

// http://learnopengl.com/code_viewer.php?code=advanced/framebuffers_screen_texture
 const GLfloat ScreenFillingQuad::quadVertices[24] = {
	// Positions   // TexCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};