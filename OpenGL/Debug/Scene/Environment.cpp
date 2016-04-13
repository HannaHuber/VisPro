#include "Environment.h"

Environment::Environment(glm::mat4& model, std::vector<Mesh*> m)
: Geometry(model, m)
{

}
Environment::~Environment() {

}