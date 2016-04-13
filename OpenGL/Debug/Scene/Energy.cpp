#include "Energy.h"

using namespace std;
using namespace glm;

Energy::Energy(mat4& model_matrix, vector<Mesh*> m, unsigned int pts)
: Item(model_matrix, m, pts)
{

}

Energy::~Energy() {

}

// Use update and draw methods from item?