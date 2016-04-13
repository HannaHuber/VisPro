#pragma once
#include "Item.h"

class Energy : public Item {
public:
	Energy(glm::mat4& model_matrix, std::vector<Mesh*> m, unsigned int pts);
	virtual ~Energy();
	virtual void update(float delta_t);
	// Use update and draw methods from item?

	// TODO shader and mesh as item attirbute? created in constructor?
};