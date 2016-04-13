#include "Energy.h"

using namespace std;
using namespace glm;

Energy::Energy(mat4& model_matrix, vector<Mesh*> m, unsigned int pts)
: Item(model_matrix, m, pts)
{

}

Energy::~Energy() {

}

void Energy::update(float delta_t) {
	for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->rotateLocally(delta_t, vec3(0.0f, 0.0f, 1.0f));
	}
}
// Use update and draw methods from item?