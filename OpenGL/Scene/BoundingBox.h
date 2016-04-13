#pragma once
#include "glm.hpp"
#include <vector>
class BoundingBox{
public:
	BoundingBox();
	~BoundingBox();
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
	std::vector<glm::vec3> allPoints;
	bool initialized;

	// Merge a bounding box into this one
	void mergeBeforeModelMatrix(BoundingBox* other);
	void mergeAfterModelMatrix(BoundingBox* other);

	void apply(glm::mat4 &model);
	
	std::vector<glm::vec3> getAllBoundingPoints();

};