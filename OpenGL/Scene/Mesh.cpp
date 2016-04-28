#include "Mesh.h"
#include "Shader.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

using namespace std;
using namespace glm;

Mesh::Mesh(std::vector<glm::vec3>& v, std::vector<glm::vec3>& n, std::vector<Face*>& f, std::vector<glm::vec2>& uv, Shader* s,  glm::mat4& model_m) // TODO
: faces(f)
, vertices(v)
, normals(n)
, v_count(v.size())
, f_count(f.size())
, shader(s)
, uvs(uv)
, model_matrix(model_m)
{ 

	// Use Shader
	shader->useShader();
	
	if (v_count < 3){
		std::cout << "OMG! There is a Mesh with less than 3 vertices! v_count=" << v_count << std::endl;
		system("PAUSE");
	}
	if (f_count < 1){
		std::cout << "OMG! There is a Mesh with less than 1 face! f_count=" << f_count << std::endl;
		system("PAUSE");
	}

	// Init index array
	index_count = 0;
	for (unsigned int i = 0; i < f_count; i++){
		// Add vertex indices per face
		vector<GLuint> indicesOfThisFace = faces[i]->getIndexVector();
		indices.insert(indices.end(), indicesOfThisFace.begin(), indicesOfThisFace.end());
		index_count += indicesOfThisFace.size();
	}

	// Init position VBO						
	glGenBuffers(1, &pos_vbo);																				// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);																	// Bind VBO
	glBufferData(GL_ARRAY_BUFFER, v_count * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);			// Copy vertex data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);																		// Unbind VBO

	// Init normal VBO						
	glGenBuffers(1, &normal_vbo);																			// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);														// Bind VBO
	glBufferData(GL_ARRAY_BUFFER, v_count * sizeof(vec3), &normals[0], GL_STATIC_DRAW);    // Copy normal data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);																// Unbind VBO

	
	// Init uv VBO						
	glGenBuffers(1, &uv_vbo);																			// Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);														// Bind VBO
	glBufferData(GL_ARRAY_BUFFER, v_count * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);// Copy uv data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);															// Unbind VBO
	

	// Init index VBO						
	glGenBuffers(1, &index_vbo);																			// Create VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);														// Bind VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);// Copy index data into VBO
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
	
	// Deactivate Shader
	glUseProgram(0);

}

Mesh::~Mesh() {
	glDeleteBuffers(1, &pos_vbo);
	glDeleteBuffers(1, &normal_vbo);
	glDeleteBuffers(1, &uv_vbo); 
	glDeleteBuffers(1, &index_vbo);
	glDeleteVertexArrays(1, &vao);

	delete shader; shader = nullptr;
} 

void Mesh::zBufferPass(ZBufferShader* z, glm::mat4& vp) {
	z->useShader();
	setUniformsForZBufferPass(z, vp);
	renderToZBuffer();
	glUseProgram(0);
}
void Mesh::setUniformsForZBufferPass(ZBufferShader* z, glm::mat4& vp) {
	auto model_location = glGetUniformLocation(z->programHandle, "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));
	auto vp_location = glGetUniformLocation(z->programHandle, "view_proj");
	glUniformMatrix4fv(vp_location, 1, GL_FALSE, glm::value_ptr(vp));
}
void Mesh::renderToZBuffer() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0); //glDrawBuffer(GL_COLOR_ATTACHMENT0); //Use GL_NONE or drawElements or drawArrays instead?
	glBindVertexArray(0);
}

void Mesh::shadingPass(const CutawaySurface* c, mat4& vp, vec3 c, vec2 planes) {
	shader->useShader();
	setUniformsForShadingPass(vp, c, sm->light_id, planes);
	draw();
	glUseProgram(0);

}
void Mesh::setUniformsForShadingPass(mat4& vp, vec3 c, int light_id, vec2 planes) {
	
	// Depth texture [activated and bound in ShadowMap::prepareZTex() for all objects]
	auto ztex_location = glGetUniformLocation(shader->programHandle, "shadow_map");
	glUniform1i(ztex_location, 2);

	// Light id (when using only one light source)
	auto id_location = glGetUniformLocation(shader->programHandle, "sm_light_id");
	glUniform1i(id_location, light_id);

	// Far plane
	auto far_location = glGetUniformLocation(shader->programHandle, "planes");
	glUniform2fv(far_location, 1, glm::value_ptr(planes));

	// View projection matrix
	auto view_proj_location = glGetUniformLocation(shader->programHandle, "view_proj");
	glUniformMatrix4fv(view_proj_location, 1, GL_FALSE, glm::value_ptr(vp));

	// Model matrix
	auto model_location = glGetUniformLocation(shader->programHandle, "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix));

	// Camera location
	auto camera_coords_location = glGetUniformLocation(shader->programHandle, "camera_coords");
	glUniform3fv(camera_coords_location, 1, glm::value_ptr(c));

	// Bind diffuse texture to unit 0
	shader->bindTexture(0);

}

void Mesh::draw() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::update(float deltaTime) {
	// Update function implemented in subclasses
}

vector<vec3> Mesh::getNormals() {
	return normals;
}

vector<vec3> Mesh::getVertices() {
	return vertices;
}

vector<GLuint> Mesh::getIndices() {
	return indices;
}
void Mesh::rotateLocally(float deltaTime, vec3 axis) {
	// Use Blender coordinate system for local transformations
	model_matrix *= rotate(mat4(1.0), deltaTime*1.0f, axis); // model matrix moved to geometry
}
void Mesh::rotateAroundCenter(vec3& world_center, float angle, vec3 axis) {
	// Rotate around center
	mat4 circle_to_world_view = translate(mat4(1.0f), world_center);
	mat4 world_to_circle_view = translate(mat4(1.0f), -world_center);
	model_matrix = circle_to_world_view * rotate(mat4(1.0), angle, axis) * world_to_circle_view * model_matrix;
}


BoundingBox* Mesh::calcBoundingBoxWithApplying(){
	BoundingBox* b = calcBoundingBoxWithOutApplying();
	b->apply(model_matrix);
	return b;
	/*float minX = 99999999999999999;
	float minY = 99999999999999999;
	float minZ = 99999999999999999;
	float maxX = -99999999999999999;
	float maxY = -99999999999999999;
	float maxZ = -99999999999999999;
	for (glm::vec3 v : vertices){
		glm::vec4 v1 = model_matrix*glm::vec4(v, 1);;
		v = vec3(v1.x / v1.w, v1.y / v1.w, v1.z / v1.w);
		minX = glm::min(minX, v.x);
		minY = glm::min(minY, v.y);
		minZ = glm::min(minZ, v.z);
		maxX = glm::max(maxX, v.x);
		maxY = glm::max(maxY, v.y);
		maxZ = glm::max(maxZ, v.z);
	}
	BoundingBox* b = new BoundingBox();
	b->minX = minX;
	b->minY = minY;
	b->minZ = minZ;
	b->maxX = maxX;
	b->maxY = maxY;
	b->maxZ = maxZ;
	//b->apply(model_matrix);
	b->allPoints.clear();
	b->allPoints.push_back(glm::vec3(minX, minY, minZ));
	b->allPoints.push_back(glm::vec3(minX, minY, maxZ));
	b->allPoints.push_back(glm::vec3(minX, maxY, minZ));
	b->allPoints.push_back(glm::vec3(minX, maxY, maxZ));
	b->allPoints.push_back(glm::vec3(maxX, minY, minZ));
	b->allPoints.push_back(glm::vec3(maxX, minY, maxZ));
	b->allPoints.push_back(glm::vec3(maxX, maxY, minZ));
	b->allPoints.push_back(glm::vec3(maxX, maxY, maxZ));
	return b;*/
}

BoundingBox* Mesh::calcBoundingBoxWithOutApplying(){
	float minX = 99999999999999999;
	float minY = 99999999999999999;
	float minZ = 99999999999999999;
	float maxX = -99999999999999999;
	float maxY = -99999999999999999;
	float maxZ = -99999999999999999;
	for (glm::vec3 v : vertices){
		minX = glm::min(minX, v.x);
		minY = glm::min(minY, v.y);
		minZ = glm::min(minZ, v.z);
		maxX = glm::max(maxX, v.x);
		maxY = glm::max(maxY, v.y);
		maxZ = glm::max(maxZ, v.z);
	}
	BoundingBox* b = new BoundingBox();
	b->minX = minX;
	b->minY = minY;
	b->minZ = minZ;
	b->maxX = maxX;
	b->maxY = maxY;
	b->maxZ = maxZ;
	//b->apply(model_matrix);
	return b;	
}

vec3 Mesh::getRelativePosition(vec3 p)  {
	return (vec3(model_matrix[3]) - p);
}

void Mesh::setShader(Shader* s) {
	shader->useShader();
	shader = s;
	glUseProgram(0);
}

void Mesh::setLighting(std::vector<std::shared_ptr<PointLight>> *allLights, int sm_light_id) {
	//shader->useShader();
	shader->setLighting(allLights , sm_light_id);
	//glUseProgram(0);
}
const float* Mesh::vec3ToFloatArray(std::list<glm::vec3*>& vectors){
	unsigned int arraySize = vectors.size()*3;
	float* result = new float[arraySize];
	unsigned int i = 0;
	for (glm::vec3* v : vectors){
		result[i++] = v->x;
		result[i++] = v->y;
		result[i++] = v->z;
	}
	if (i != arraySize){
		std::cout << "You've done it wrong! converting vector coordinates into float array in mesh.cpp doesn't work as expexted. info:" << i << arraySize << std::endl;
	}
	return result;
}
