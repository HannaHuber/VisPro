#pragma once
#include "Item.h"

class Energy : public Item {
public:
	Energy(glm::mat4& model_matrix, std::vector<Mesh*> m, unsigned int pts);
	virtual ~Energy();
	// Use update and draw methods from item?

	// TODO shader and mesh as item attirbute? created in constructor?
};