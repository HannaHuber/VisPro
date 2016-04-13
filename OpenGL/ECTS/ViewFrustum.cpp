#include "ViewFrustum.h"

ViewFrustum::ViewFrustum(){
	for (int i = 0; i < 6; i++){
		clipping_planes[i] = ViewFrustumPlane();
	}
}

ViewFrustum::~ViewFrustum(){

}

void ViewFrustum::defineCamera(float& nearDist, float& farDist, float& fieldOfView, float& ratio){
	near_Dist = nearDist;
	far_Dist = farDist;
	fov = fieldOfView;
	rat = ratio;
}

//Don't forget to call defineCamera before!
void ViewFrustum::calcFrustum(glm::mat4& modelViewMat){
	
	camPos = glm::vec3(inverse(modelViewMat)[3]); //https://www.opengl.org/discussion_boards/showthread.php/178484-Extracting-camera-position-from-a-ModelView-Matrix
	camViewRayNormalized = normalize(glm::vec3(modelViewMat[0][2], modelViewMat[1][2], modelViewMat[2][2])); //http://stackoverflow.com/questions/15697273/how-can-i-get-view-direction-from-the-opengl-modelview-matrix
	//up = normalize(glm::vec3(modelViewMat[0][1], modelViewMat[1][1], modelViewMat[2][1]));
	right = normalize(glm::vec3(modelViewMat[0][0], modelViewMat[1][0], modelViewMat[2][0]));
	up = normalize(glm::cross(camViewRayNormalized,right)); // cross product of view and up vector
	//up = glm::cross(camViewRayNormalized,right); // cross product of view and up vector
	//up = normalize(up);

	/*double ANG2RAD = 3.14159265358979323846 / 180.0;

	double tang = (float)tan(fov* ANG2RAD * 0.5);
	near_Height = near_Dist * tang;
	near_Width = near_Height * rat;
	far_Height = far_Dist  * tang;
	far_Width = far_Height * rat;
	*/
	near_Height = 2 * tan(fov / 2)*near_Dist;
	near_Width = near_Height * rat;
	far_Height = 2 * tan(fov / 2)*far_Dist;
	far_Width = far_Height * rat;


	createPlanes();


	//TODO: Notiz an mich: Testen ist möglich indem near, far plane und fov falsch/verschoben/kleiner übergeben werden;
}
void ViewFrustum::createPlanes(){
	//calculate points for planes
	glm::vec3 near_mid = camPos + near_Dist * camViewRayNormalized;
	glm::vec3 far_mid = camPos + far_Dist * camViewRayNormalized;

	glm::vec3 near_bottom_left = near_mid - ((near_Height / 2) * up) - ((near_Width / 2) * right);
	glm::vec3 near_bottom_right = near_mid - ((near_Height / 2) * up) + ((near_Width / 2) * right);
	glm::vec3 near_top_right = near_mid + ((near_Height / 2) * up) + ((near_Width / 2) * right);

	glm::vec3 far_bottom_left = far_mid - ((far_Height / 2) * up) - ((far_Width / 2) * right);
	glm::vec3 far_top_left = far_mid + ((far_Height / 2) * up) - ((far_Width / 2) * right);
	glm::vec3 far_top_right = far_mid + ((far_Height / 2) * up) + ((far_Width / 2)* right);

	// Set planes
	clipping_planes[0].init(near_bottom_left,near_top_right,near_bottom_right);
	clipping_planes[1].init(near_bottom_right, near_top_right, far_top_right);
	clipping_planes[2].init(far_top_right, far_bottom_left, far_top_left);
	clipping_planes[3].init(far_top_left, near_bottom_left, far_bottom_left);
	clipping_planes[4].init(far_top_left, far_top_right, near_top_right);
	clipping_planes[5].init(near_bottom_left, near_bottom_right, far_bottom_left);
}

bool ViewFrustum::isCullable(BoundingBox* b){
	std::vector<glm::vec3> points = b->getAllBoundingPoints();
	for (ViewFrustumPlane plane : clipping_planes){
	//ViewFrustumPlane plane = clipping_planes[0]; //for debugging
	bool allOutside = true;
		for (glm::vec3 p : points){
			//std::cout << "c2(" << int(camPos.x) << "," << int(camPos.y) << "," << int(camPos.z) << ")" << std::endl;
			allOutside = allOutside&&plane.isOutside(p);
			//std::cout <<"outside: "<< plane.isOutside(p) << std::endl;
		}
		if (allOutside){
			//std::cout << "all outside plane " << i << std::endl;
			return true;
		}
	}
	//std::cout << "Not all Outside!"<<std::endl;
	return false;

}


////////////////////////////////ViewFrustumPlane////////////////////////////////////

ViewFrustum::ViewFrustumPlane::ViewFrustumPlane(){

}
ViewFrustum::ViewFrustumPlane::~ViewFrustumPlane(){

}
void ViewFrustum::ViewFrustumPlane::init(glm::vec3 a, glm::vec3 b, glm::vec3 c){
	normalvector = normalize(glm::cross(b - a,c - a));
	d = -(a.x*normalvector.x + a.y*normalvector.y + a.z*normalvector.z);
}
bool ViewFrustum::ViewFrustumPlane::isOutside(glm::vec3 point){
	//std::cout << "p(" << int(point.x) << "," << int(point.y) << "," << int(point.z) << ") - n(" << int(round(normalvector.x)) << "," << int(round(normalvector.y)) << "," << int(round(normalvector.z)) << ")" << std::endl;
	//std::cout << "p(" << int(point.x) << "," << int(point.y) << "," << int(point.z) << ")" << std::endl;
	return 0>(point.x*normalvector.x + point.y*normalvector.y + point.z*normalvector.z + d); 
}