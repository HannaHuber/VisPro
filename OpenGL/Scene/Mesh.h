#pragma once

#include "glm.hpp"
#include <list>
#include <vector>

#include "Shader.h"
#include "../ECTS/ShadowMap.h"
#include "Face.h"
#include "BoundingBox.h"

class Mesh {
public:
	glm::mat4 model_matrix;

	// Constructor + destructor
	Mesh::Mesh(std::vector<glm::vec3>& v, std::vector<glm::vec3>& n, std::vector<Face*>& f, std::vector<glm::vec2>& uv, Shader* shader ,glm::mat4& model_m); 
	virtual ~Mesh();

	// Update methods
	virtual void update(float delta_t);
	void rotateLocally(float delta_t, glm::vec3 axis);
	void rotateAroundCenter(glm::vec3& world_center, float angle, glm::vec3 axis);

	// Needed for Physics
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getVertices();
	std::vector<GLuint> getIndices();

	// Z buffer pass methods
	void zBufferPass(ZBufferShader* z);
	void setUniformsForZBufferPass(ZBufferShader* z);
	void renderToZBuffer();


	// Shading pass methods
	void shadingPass(const ShadowMap* sm, glm::mat4& vp, glm::vec3 c, glm::vec2 planes);
	void setUniformsForShadingPass(glm::mat4& vp, glm::vec3 c, int id, glm::vec2 planes);
	virtual void draw();
		
	// Initial settings
	void setLighting(std::vector<std::shared_ptr<PointLight>> *allLights, int sm_light_id);
	void setShader(Shader* s);

	BoundingBox* Mesh::calcBoundingBoxWithApplying();
	BoundingBox* Mesh::calcBoundingBoxWithOutApplying();
	glm::vec3 getRelativePosition(glm::vec3 p);

	GLuint f_count; //number of faces
private:

	// Vertex data
	std::vector<glm::vec3> vertices;		// Vertex positions
	std::vector<glm::vec3> normals;			// Vertex normals
	std::vector<glm::vec2> uvs;				// Vertex uv coords
	std::vector<GLuint> indices;			// Vertex indices
	GLuint v_count, index_count;	// Counts

	// Faces
	const std::vector<Face*> faces; 		
	
	// Vertex buffer objects
	GLuint pos_vbo, normal_vbo, uv_vbo, index_vbo;

	// Vertex array object
	GLuint vao;

	// Shader
	Shader* shader;

	const float* Mesh::vec3ToFloatArray(std::list<glm::vec3*>& vectors);



};