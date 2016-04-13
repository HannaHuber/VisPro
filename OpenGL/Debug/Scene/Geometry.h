#pragma once

#include "SceneObject.h"
#include "Mesh.h"
#include "../ECTS/ShadowMap.h"
#include <vector>
#include <memory>
#include "../ECTS/ViewFrustum.h"

class Geometry : public SceneObject{
public:
	Geometry(glm::mat4& model_matrix, std::vector<Mesh*> m);
	virtual ~Geometry();
	virtual void update(float deltaTime);
	int Geometry::draw(const ShadowMap* sm, ViewFrustum& frust, glm::mat4& vp, glm::vec3 c, float f, bool useViewFrustumCulling);
		virtual void draw();
	virtual void addChild(Geometry* g);
	virtual void setShader(Shader* shader);
	virtual void transformModelMatrix(glm::mat4& transform );
	virtual void setLighting(std::vector<std::shared_ptr<PointLight>> *allLights, int sm_light_id);
	virtual void setViewProj(glm::mat4& vpm);
	virtual void setCameraLoc(glm::vec3 loc);
	virtual void renderToZBuffer(ZBufferShader* z);
	std::vector<Mesh*> meshes;
	int faceCount =0; //number of faces in this Geometry
private:

	BoundingBox* b; 

	
};

