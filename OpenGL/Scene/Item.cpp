#include "Item.h"
#include "gtc/matrix_transform.hpp"


using namespace std;
using namespace glm;

Item::Item(mat4& model_matrix, std::vector<Mesh*> m, unsigned int pts)
: Geometry(model_matrix, m)
, points(pts)
{

}

Item::~Item() {

}


void Item::update(float delta_t) {
	// Update object matrix
	model_matrix *= rotate(mat4(1.0), delta_t*1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix = translate(mat4(1.0), 2.0f*delta_t*slope) * model_matrix;

	// Update mesh matrices
	for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->rotateLocally(delta_t, vec3(0.0f, 0.0f, 1.0f));															// Rotate around vertikal axis
		meshes[i]->model_matrix = translate(mat4(1.0), 2.0f*delta_t*slope) * meshes[i]->model_matrix;// Translate along line (used for question items)
	}
}

void Item::setSlope(vec3& g) {
	slope = g;
}