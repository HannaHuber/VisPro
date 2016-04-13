#pragma once
#include "Geometry.h"
#include "Camera.h"
#include "Knowledge.h"
#include "Energy.h"


class Player : public Geometry{
public:
	Player(GLFWwindow* window, glm::mat4& model_matrix, std::vector<Mesh*> meshes);
	~Player();
	virtual void update(float deltaT);
	virtual void draw();
	glm::vec3 handleInput(float deltaT);
	glm::vec3 getMoveInput();
	glm::vec2 getLookInput();
	bool getFireInput();
	void move(glm::vec3 direction, float deltaT);
	void lookAt(glm::vec2 angle);
	bool is_firing;
	std::vector<std::shared_ptr<Knowledge>> bullets;
	void collectItem(std::shared_ptr<Knowledge> i);
	void collectItem(std::shared_ptr<Energy> i);
	void collideWithStudent();
	void passExam(float points);
	unsigned int getEnergyStatus();
	unsigned int getKnowledgeStatus();
	Camera* camera;
	
	float last_bullet_fired, min_time;

private:
	float ECTS, knowledge, energy;
	GLFWwindow* window;
	double screen_center_x, screen_center_y;
	float speed, look_speed;

	float lastJumpPress = 0;
	float minJumpInterval = 2.2f;
	float jump_velocity = 0.65f;

};