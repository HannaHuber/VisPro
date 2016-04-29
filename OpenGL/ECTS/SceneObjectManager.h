#pragma once

#include "PointLight.h"
#include "Camera.h"
#include "Environment.h"
#include "Energy.h"
#include "UserInput.h"

class SceneObjectManager {
public:
	SceneObjectManager();
	~SceneObjectManager();
	void setObjects(Camera* c,
					std::vector<std::shared_ptr<PointLight>>* l, 
					std::vector<std::shared_ptr<Environment>>* e, 
					std::vector<std::shared_ptr<Energy>>* e_i);
	void setLighting();
	
	void update(float delta_t);
	void renderToZBuffer(ZBufferShader* z, glm::mat4& vp);
	int draw(const CutawaySurface* sm, bool useViewFrustumCulling);

private:
	std::vector<std::shared_ptr<PointLight>>* lights;
	Camera* camera;
	std::vector<std::shared_ptr<Environment>>* environment;
	std::vector<std::shared_ptr<Energy>>* e_items;
	ViewFrustum frustum = ViewFrustum();
	

};
