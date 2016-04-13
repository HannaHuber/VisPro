#pragma once

#include "ZBufferShader.h"
#include "PointLight.h"
#include <memory>

class ShadowMap {
public: 
	ShadowMap(int ww, int hh, float fov, float near, float far);
	~ShadowMap();

	void init();
	void setLight(std::shared_ptr<PointLight> l, int id);
	void prepareZBufferPass();
	void endZBufferPass();
	void prepareShadingPass(int unit);
	
	glm::mat4 view[6];
	glm::mat4 proj;
	glm::vec2 planes;
	int light_id;
	std::shared_ptr<PointLight> light;
	ZBufferShader* z_buffer_shader;
private:
	GLuint depth_cube, fb;
	int width, height;
};