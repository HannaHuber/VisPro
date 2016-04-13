#include "Player.h"
#include "gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;

Player::Player(GLFWwindow* window, mat4& model, std::vector<Mesh*> meshes) 
: Geometry(model, meshes)
, window(window)
, ECTS(0)
, knowledge(0)
, energy(100)
//TODO ,bullets("EMPTY_LIST")
, camera( new Camera(model))
, speed(5.0f) // standard moving speed
, look_speed(0.001f) // standard camera rotation speed
, last_bullet_fired(-1.0f)
, min_time(1.0)
{
	// Define screen center position for offset calculation in update()
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	screen_center_x = width / 2.0;
	screen_center_y = height / 2.0;
	
}

Player::~Player() {
	delete camera; camera = nullptr;
}

void Player::draw()  {
	// First person --> draw nothing
}

void Player::update(float deltaT) {

	// Input handling: Actions
	handleInput(deltaT);

	// Collision detection: Reactions
	// TODO: update/count energy pts

	// TODO: update/count knowledge pts
	// TODO: update bullet collisions
	// TODO: update student collisions
	

}

vec3 Player::handleInput(float deltaT) {
	
	vec2 angle = getLookInput();	 // Look around	
	lookAt(angle);
		
	vec3 direction = getMoveInput(); // Move in physics class
	//move(direction, deltaT);

	is_firing = getFireInput();  // Fire

	/*if (collected_energy.size() > 0) {
		if (time_to_clear_energy > 0) {
			time_to_clear_energy -= 1;
		}
		else {
			collected_energy.clear();
			time_to_clear_energy = 5;
		}
	}*/
	
	return direction;

}

// not used?
void Player::move(vec3 direction, float deltaT) {	
	model_matrix *=  translate(mat4(1.0), speed*deltaT*direction); // Translate player
	camera->update(speed*deltaT, direction);						 // Translate camera
}

void Player::lookAt(vec2 angle) {
	
	// Rotate player 
	mat4 T = translate(mat4(1.0), -location());											// Rotate horizontally:Translate to origin
	T = rotate(mat4(1.0), look_speed*(float)angle.x, vec3(0.0f, 1.0f, 0.0f)) * T;			// Rotate around y-axis
	model_matrix = translate(mat4(1.0), location()) * T * model_matrix;					// Translate back to position
	model_matrix *= rotate(mat4(1.0), look_speed*(float)angle.y, vec3(-1.0f, 0.0f, 0.0f));  // Rotate vertically

	// Rotate camera
	camera->update(look_speed,angle);

	return;
}

vec3  Player::getMoveInput() {
	// Check key input
	vec3 direction = vec3(0.0);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		direction = camera->forward_vector();				// move forward
		direction = vec3(direction.x, 0, direction.z);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//direction = vec3(-1.0, 0.0, 0.0);					// move left
		direction = camera->left_vector();
		direction = vec3(direction.x, 0, direction.z);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//direction = vec3(0.0, 0.0, 1.0);					// move backward
		direction = camera->backward_vector();		
		direction = vec3(direction.x, 0, direction.z);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//direction = vec3(1.0, 0.0, 0.0);					// move right
		direction = camera->right_vector();
		direction = vec3(direction.x, 0, direction.z);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (glfwGetTime() - lastJumpPress > minJumpInterval){
			direction += vec3(0.0, jump_velocity*1.0, 0.0);					// jump (move upwards)
			lastJumpPress = glfwGetTime();
		}
	}
	
	return direction;

}

vec2 Player::getLookInput() {
	
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);							// Get cursor position
	glfwSetCursorPos(window, screen_center_x, screen_center_y);		// Set cursor position for next frame

	// return horizontal and vertical angle
	return vec2(screen_center_x - xpos, ypos- screen_center_y );
	
}

bool Player::getFireInput() {
	// check mouseclick 
	bool click = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
	// check time since last fire
	bool time_ok = ((glfwGetTime() - last_bullet_fired) > 0);
	if (time_ok) {
		last_bullet_fired = glfwGetTime();
	}
	return (click && time_ok);
}

void Player::collectItem(std::shared_ptr<Knowledge> i) {
	knowledge += i->points;							// Get knowledge points from collected knowledge item
	cout << "Congratulations! You collected an knowledge item and gained " << i->points << " knowledge points!" << endl;
	cout << "Current knowledge status: " << knowledge << endl;
	//std::shared_ptr<Knowledge> i_shared(i);
	bullets.push_back(i);
}

void Player::collectItem(std::shared_ptr<Energy> i) {
	energy += i->points;							// Get energy points from collected energy item
	cout << "Congratulations! You collected an energy item and gained " << i->points << " energy points!" << endl;
	cout << "Current energy status: " << energy << endl;
	
}

void Player::collideWithStudent() {
	energy -= 5;									// Lose energy points from collision with student
	cout << "Watch out! You collided with a student and lost " << 5 << " energy points!" << endl;
	cout << "Current energy status: " << getEnergyStatus() << endl;

}

void Player::passExam(float points) {
	cout << "Yeah, you just passed this exam worth: " << points << " ECTS!"<<std::endl;
	ECTS += points;
}

unsigned int Player::getEnergyStatus() {
	return energy;
}

unsigned int Player::getKnowledgeStatus() {
	return knowledge;
}