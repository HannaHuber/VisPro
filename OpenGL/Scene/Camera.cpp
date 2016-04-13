#include "Camera.h"
#include "gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;

Camera::Camera(glm::mat4& model)
: SceneObject(model)
{
	
}

Camera::~Camera() {

}

// Rotate horizontally and vertically
void Camera::update(float look_speed, vec2 angle) {
	
	vec3 position = vec3(model_matrix[3]);

	// rotate horizontally
	mat4 T = translate(mat4(1.0), -position);										// Translate to origin
	T = rotate(mat4(1.0), (float)(look_speed*angle.x), vec3(0.0f, 1.0f, 0.0f)) * T;	// Rotate around y-axis
	model_matrix = translate(mat4(1.0),position) * T * model_matrix;				// Translate back to position
		
	// rotate vertically
	model_matrix = model_matrix * rotate(mat4(1.0), (float)(look_speed*angle.y), vec3(-1.0, 0.0, 0.0));
	
}

// Translate in specified direction
void Camera::update(float deltaTime, vec3 direction) {
	model_matrix = model_matrix * translate(mat4(1.0), deltaTime*direction);
	
}

void Camera::update(float deltaTime) {

}

void Camera::draw() {

}

mat4 Camera::view_matrix() {
	return inverse(model_matrix);
}
vec3 Camera::forward_vector() {
	mat4 v = view_matrix();
	return vec3(v[2].x, v[2].y, -v[2].z);
}
vec3 Camera::backward_vector(){
	mat4 v = view_matrix();
	return vec3(-v[2].x, -v[2].y, v[2].z);
}
vec3 Camera::right_vector() {
	mat4 v = view_matrix();
	return vec3(v[0].x, -v[0].y, -v[0].z);
}
vec3 Camera::left_vector() {
	mat4 v = view_matrix();
	return vec3(-v[0].x, v[0].y, v[0].z);
}



vec3 Camera::camera_location(){
	return vec3(model_matrix[3]);
}

void Camera::setProjMatrix(int w, int h, float f, float n_plane, float f_plane) {
	fov = f;
	ratio = w / (float)h;
	planes = vec2(n_plane,f_plane);
	proj_matrix = glm::perspective(f, w / (float)h, n_plane, f_plane);
}