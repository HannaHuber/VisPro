#include "Student.h"
using namespace glm;
using namespace std;

Student::Student(mat4& model, vector<Mesh*> meshes)
: Geometry(model, meshes)
{

}

Student::~Student() {

}
