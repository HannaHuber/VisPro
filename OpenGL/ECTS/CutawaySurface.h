#pragma once

#include "ZBufferShader.h"
#include "Quad.h"
#include "QuadShader.h"
#include <memory>

class CutawaySurface {
public: 
	CutawaySurface();
	~CutawaySurface();

	void init(int w, int h, float z_near, float z_far, float angle);
	void prepareZBufferPass();
	void endZBufferPass();
	void prepareQuadPass(int step, glm::mat4& vp);
	void prepareShadingPass(int unit);
	
	ZBufferShader* z_buffer_shader;
	Quad* quad;
	QuadShader* quad_shader;
private:
	GLuint depth_map, temp_tex, look_up_tex, fbo1, fbo2, target_fbo;
	int width, height, last_target;
};