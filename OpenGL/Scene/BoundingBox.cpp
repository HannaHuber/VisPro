#include "BoundingBox.h"

BoundingBox::BoundingBox():
initialized(false){
	
}
BoundingBox::~BoundingBox(){

}
void BoundingBox::mergeBeforeModelMatrix(BoundingBox* other){
	minX = glm::min(minX, other->minX);
	minY = glm::min(minY, other->minY);
	minZ = glm::min(minZ, other->minZ);
	maxX = glm::max(maxX, other->maxX);
	maxY = glm::max(maxY, other->maxY);
	maxZ = glm::max(maxZ, other->maxZ);
}
void BoundingBox::mergeAfterModelMatrix(BoundingBox* other){
	minX = 99999999999999999.9f;
	minY = 99999999999999999.9f;
	minZ = 99999999999999999.9f;
	maxX = -99999999999999999.9f;
	maxY = -99999999999999999.9f;
	maxZ = -99999999999999999.9f;
	for (glm::vec3 v : allPoints){
		minX = glm::min(minX, v.x);
		minY = glm::min(minY, v.y);
		minZ = glm::min(minZ, v.z);
		maxX = glm::max(maxX, v.x);
		maxY = glm::max(maxY, v.y);
		maxZ = glm::max(maxZ, v.z);
	}
	for (glm::vec3 v : other->allPoints){
		minX = glm::min(minX, v.x);
		minY = glm::min(minY, v.y);
		minZ = glm::min(minZ, v.z);
		maxX = glm::max(maxX, v.x);
		maxY = glm::max(maxY, v.y);
		maxZ = glm::max(maxZ, v.z);
	}
	allPoints.clear();
	allPoints.push_back(glm::vec3(minX, minY, minZ));
	allPoints.push_back(glm::vec3(minX, minY, maxZ));
	allPoints.push_back(glm::vec3(minX, maxY, minZ));
	allPoints.push_back(glm::vec3(minX, maxY, maxZ));
	allPoints.push_back(glm::vec3(maxX, minY, minZ));
	allPoints.push_back(glm::vec3(maxX, minY, maxZ));
	allPoints.push_back(glm::vec3(maxX, maxY, minZ));
	allPoints.push_back(glm::vec3(maxX, maxY, maxZ));
}

void BoundingBox::apply(glm::mat4 &model){
	allPoints.clear();
	glm::vec4 v1 = model*glm::vec4(minX, minY, minZ, 1);
	allPoints.push_back(glm::vec3(v1/v1.w));
	glm::vec4 v2 = model*glm::vec4(minX, minY, maxZ, 1);
	allPoints.push_back(glm::vec3((v2 / v2.w)));
	glm::vec4 v3 = model*glm::vec4(minX, maxY, minZ, 1);
	allPoints.push_back(glm::vec3((v3 / v3.w)));
	glm::vec4 v4 = model* glm::vec4(minX, maxY, maxZ, 1);
	allPoints.push_back(glm::vec3((v4 / v4.w)));
	glm::vec4 v5 = model*glm::vec4(maxX, minY, minZ, 1);
	allPoints.push_back(glm::vec3((v5 / v5.w)));
	glm::vec4 v6 = model*glm::vec4(maxX, minY, maxZ, 1);
	allPoints.push_back(glm::vec3((v6 / v6.w)));
	glm::vec4 v7 = model* glm::vec4(maxX, maxY, minZ, 1);
	allPoints.push_back(glm::vec3((v7 / v7.w)));
	glm::vec4 v8 = model*glm::vec4(maxX, maxY, maxZ, 1);
	allPoints.push_back(glm::vec3((v8 / v8.w)));

	minX = 99999999999999999.9f;
	minY = 99999999999999999.9f;
	minZ = 99999999999999999.9f;
	maxX = -99999999999999999.9f;
	maxY = -99999999999999999.9f;
	maxZ = -99999999999999999.9f;
	for (glm::vec3 v : allPoints){
		minX = glm::min(minX, v.x);
		minY = glm::min(minY, v.y);
		minZ = glm::min(minZ, v.z);
		maxX = glm::max(maxX, v.x);
		maxY = glm::max(maxY, v.y);
		maxZ = glm::max(maxZ, v.z);
	}

	initialized = true;
}

std::vector<glm::vec3> BoundingBox::getAllBoundingPoints(){
	return allPoints;
}
