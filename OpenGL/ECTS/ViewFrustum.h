#pragma once
#include "glm.hpp"
#include "BoundingBox.h"
#include <iostream>


class ViewFrustum{
public:
	ViewFrustum();
	~ViewFrustum();
	void defineCamera(float& nearDist, float& farDist, float& fieldOfView, float& ratio);
	void calcFrustum(glm::mat4& modelViewMat);
	bool ViewFrustum::isCullable(BoundingBox* b);

	class ViewFrustumPlane{
	public:
		ViewFrustumPlane();
		~ViewFrustumPlane();
		void init(glm::vec3 a, glm::vec3 b, glm::vec3 c);
		bool isOutside(glm::vec3 point);
	private:
		glm::vec3 normalvector;
		float d; // ax+by+cz+d = 0
	};

private:
	float fov;
	float rat;
	glm::vec3 camPos;
	glm::vec3 camViewRayNormalized;
	float near_Dist;
	float far_Dist;
	float near_Height;
	float near_Width;
	float far_Height;
	float far_Width;
	glm::vec3 up;
	glm::vec3 right;

	//0:near, 1:right, 2:far, 3:left, 4:top, 5:bottom 
	ViewFrustumPlane clipping_planes[6];

	void ViewFrustum::createPlanes();


	
};