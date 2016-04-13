#pragma once
#include "Geometry.h"

class Professor : public Geometry {
public:
	Professor(glm::mat4& model, std::vector<Mesh*> meshes, float e, glm::vec3& wcc, int head);
	virtual ~Professor();
	virtual void update(float delta_t);
	void throwQuestion(); // Implement in physics class?
	float hits, ects;
private:
	float path;
	glm::vec3 world_circle_center;
	int head;
};