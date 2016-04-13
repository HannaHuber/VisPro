#pragma once
#include "Geometry.h"

class Environment : public Geometry {
public: 
	Environment(glm::mat4& model_matrix, std::vector<Mesh*> m);
	~Environment();
	std::vector<glm::vec3> world_vertices;
};