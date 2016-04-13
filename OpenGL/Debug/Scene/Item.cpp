#include "Item.h"

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
	for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->rotateLocally(delta_t);
	}
}

// Use draw method from Geometry?