#include "SceneObjectManager.h"


SceneObjectManager::SceneObjectManager() {

}
SceneObjectManager::~SceneObjectManager() {

}

// Set scene objects
void SceneObjectManager::setObjects(Camera* c,
									std::vector<std::shared_ptr<PointLight>>* l, 
									std::vector<std::shared_ptr<Environment>>* e,
									std::vector<std::shared_ptr<Energy>>* e_i) {
	lights = l;
	camera = c;
	environment = e;
	e_items = e_i;
	frustum.defineCamera(camera->planes.x, camera->planes.y, camera->fov, camera->ratio);
}

// Set light parameters for drawable objects
void SceneObjectManager::setLighting() {
	for (std::shared_ptr<Environment> e : *environment) {				// Set lighting for environment
		e->setLighting(lights);
	}
	for (std::shared_ptr<Energy> e : *e_items) {
		e->setLighting(lights);								// Set lighting for energy items
	}
}

// Render depth values to z buffer (depth map)
void SceneObjectManager::renderToZBuffer(ZBufferShader* z, glm::mat4& vp) {
	
	for (std::shared_ptr<Energy> e : *e_items) {
		e->renderToZBuffer(z, vp);									 // Render depth values for energy items
	}
}

// Update all objects
void SceneObjectManager::update(float delta_t) {
	for (std::shared_ptr<Environment> e : *environment) {		// Update environment
		e->update(delta_t);
	}
	for (std::shared_ptr<Energy> e : *e_items) {				// Update energy items
		e->update(delta_t);									
	}	
}

// Draw drawable objects. Return number of drawn faces
int SceneObjectManager::draw(const CutawaySurface* sm, bool useViewFrustumCulling) {
	glm::mat4 vpm = camera->proj_matrix * camera->view_matrix();// Calculate view projection matrix from camera
	glm::vec3 loc = camera->location();
	
	if (useViewFrustumCulling){
		frustum.calcFrustum(camera->view_matrix());
	}
	
	int faces = 0;
	for (std::shared_ptr<Environment> e : *environment) {		// Draw environment
		//faces += e->draw(sm, frustum, vpm, loc, f, useViewFrustumCulling);
		faces += e->draw(sm, frustum, vpm, loc, useViewFrustumCulling);
	}	

	//Important: draw transparent objects (energy items) after all other objects
	if (useTransparency){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glDisable(GL_CULL_FACE);
	}
	else{
		glDisable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	for (std::shared_ptr<Energy> e : *e_items) {				// Draw energy items
		faces += e->draw(sm, frustum, vpm, loc, false);// do not use view frustum culling because these objects are moving and have only few faces
	}

	glEnable(GL_CULL_FACE);
	return faces;
}