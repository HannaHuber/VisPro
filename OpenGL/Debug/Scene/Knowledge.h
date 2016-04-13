#pragma once
#include "Item.h"

class Knowledge : public Item {
public:
	Knowledge(glm::mat4& model_matrix, std::vector<Mesh*> m, unsigned int pts);
	virtual ~Knowledge();
	// Use update and draw methods from item?
};