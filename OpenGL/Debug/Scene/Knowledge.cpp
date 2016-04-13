#include "Knowledge.h"

using namespace std;
using namespace glm;

Knowledge::Knowledge(mat4& model_matrix, vector<Mesh*> m, unsigned int pts)
: Item(model_matrix, m, pts)
{

}

Knowledge::~Knowledge() {

}

// Use update and draw methods from item?