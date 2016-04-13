#include "Professor.h"
#include "gtc/matrix_transform.hpp"
using namespace glm;

Professor::Professor(mat4& model, std::vector<Mesh*> meshes, float e, vec3& wcc, int h)
: Geometry(model, meshes)
, ects(e)
, world_circle_center(wcc)
, head(h)
, path(0.0f)
{
	
}

Professor::~Professor() {

}

void Professor::update(float delta_t) {
	path += delta_t/100.0;
	for (int i = 0; i < meshes.size(); i++) {
		if (i == head) {
			meshes[i]->rotateLocally(sin(path));									// Shaking head
		}
		meshes[i]->rotateAroundCenter(world_circle_center, delta_t, vec3(0,1,0));	// Update relative to professor
	}
	
}