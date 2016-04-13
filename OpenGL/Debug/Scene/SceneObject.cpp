#include "SceneObject.h"

#include "glm.hpp"

// Constructor
SceneObject::SceneObject(glm::mat4& model)
: model_matrix(model) {	

}

// Destructor
SceneObject::~SceneObject() {
	
}

glm::vec3 SceneObject::location() {
	return glm::vec3(model_matrix[3]);
}

