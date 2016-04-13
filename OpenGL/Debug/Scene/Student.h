# pragma once
#include "Geometry.h"

class Student : public Geometry {
public: Student(glm::mat4& model, std::vector<Mesh*> meshes);
		~Student();

};