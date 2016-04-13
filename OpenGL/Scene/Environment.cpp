#include "Environment.h"
using namespace glm;
using namespace std;

Environment::Environment(glm::mat4& model, std::vector<Mesh*> m)
: Geometry(model, m)
{
// Calculate world space vertex position of first mesh (suppose a plane only has one mesh)
	for (vec3 v : m[0]->getVertices()) {
		vec3 v_world = vec3(m[0]->model_matrix * vec4(v,1));
		world_vertices.push_back(v_world);
	}
}
Environment::~Environment() {

}
