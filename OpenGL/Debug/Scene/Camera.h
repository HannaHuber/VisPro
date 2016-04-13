#pragma once

#include "SceneObject.h"
#include "glm.hpp"

class Camera : public SceneObject {
public:
	Camera(glm::mat4& model_matrix);
	virtual ~Camera();
	virtual void update(float look_speed, glm::vec2 angle);			// rotate
	virtual void update(float deltaTime, glm::vec3 direction);		// translate
	virtual void update(float deltaTime);
	virtual void draw();
	virtual glm::vec3 camera_location();
	glm::vec2 planes;
	float fov, ratio;

	void setProjMatrix(int w, int h, float fov, float far_plane, float near_plane);

	glm::mat4 view_matrix();
	glm::vec3 forward_vector();
	glm::vec3 backward_vector();
	glm::vec3 right_vector();
	glm::vec3 left_vector();

	glm::mat4 proj_matrix;

	

};