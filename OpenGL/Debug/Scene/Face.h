#pragma once
#include <vector>
#include "glew.h"


class Face {
public:
	Face(unsigned int v_count, unsigned int* v_indices);
	~Face();
	std::vector<GLuint> getIndexVector();
	unsigned int v_count;
	unsigned int* v_indices;
};