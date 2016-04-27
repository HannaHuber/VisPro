#include "Geometry.h"
#include "gtc/matrix_transform.hpp"
#include <iostream>

using namespace glm;
using namespace std;

Geometry::Geometry(glm::mat4& model, std::vector<Mesh*> m)
: SceneObject(model)
, meshes(m)
, b(new BoundingBox()){
	
	for (unsigned int i = 0; i < meshes.size(); i++){
		faceCount += meshes[i]->f_count;						// Init facecount
		vec3 rel = meshes[i]->getRelativePosition(location());	// Init relative mesh positions
		relativeMeshPositions.push_back(rel);
	}

}
Geometry::~Geometry() {
	delete b;
}
void Geometry::update(float deltaTime){
	for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->update(deltaTime);
	}
	
}

/*Returns the number of faces that have been drawn. Is 0 if Geometry is culled*/
int Geometry::draw(const ShadowMap* sm, ViewFrustum& frust, glm::mat4& vp, glm::vec3 c, glm::vec2 planes, bool useViewFrustumCulling){
	if (useViewFrustumCulling){
		if (!b->initialized){ // only calc bounding box if not done yet (performance ;) - objects are not moving)
			delete b;
			
			/*b = meshes[0]->calcBoundingBoxWithOutApplying();

			for (unsigned int i = 1; i < meshes.size(); i++){
				BoundingBox* b2 = meshes[i]->calcBoundingBoxWithOutApplying();
				b->mergeBeforeModelMatrix(b2);
				delete b2;
			}
			b->apply(meshes[0]->model_matrix);
		*/
			b = meshes[0]->calcBoundingBoxWithApplying();

			for (unsigned int i = 1; i < meshes.size(); i++){
				BoundingBox* b2 = meshes[i]->calcBoundingBoxWithApplying();
				b->mergeAfterModelMatrix(b2);
				delete b2;
			}
			//b->apply(meshes[0]->model_matrix);
		}
		if (frust.isCullable(b)){
			return 0;
		}
	}
	for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->shadingPass(sm, vp, c, planes);
	}
	return faceCount;
}

void Geometry::draw() {
	// Necessary due to abstract class SceneObject
}
void Geometry::addChild(Geometry* child){
	//TODO
	// Note: if you implement this, you might háve to change the SceneImporter or ignore the list of all geometries, which is created there.
	//		If you decide to draw all geometries in this list and recursively also all children of this geometry, you will draw some geomtries twice.
}
void Geometry::setShader(Shader* shader){
	for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->setShader(shader);
	}
}

void Geometry::transformModelMatrix(mat4& transform){
	model_matrix = transform*model_matrix; // transform locally
	for (unsigned int i = 0; i < meshes.size(); i++){
		// transform mesh model_matrices
		meshes[i]->model_matrix = transform * meshes[i]->model_matrix;
	}
}
void Geometry::setLighting(std::vector<std::shared_ptr<PointLight>> *allLights, int sm_light_id){
	for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->setLighting(allLights, sm_light_id);
	}
}
void Geometry::setViewProj(mat4& vpm){
	/*for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->setViewProj(vpm);
	}*/
}
void Geometry::setCameraLoc(glm::vec3 loc){
	/*for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->setCameraLoc(loc);
	}*/
}

void Geometry::renderToZBuffer(ZBufferShader* z, glm::mat4& vp) {
	for (unsigned int i = 0; i < meshes.size(); i++){
		meshes[i]->zBufferPass(z, vp);
	}

}