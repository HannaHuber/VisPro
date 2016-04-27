#pragma once

#include "ZBufferShader.h"
#include <memory>

class CutawaySurface {
public: 
	CutawaySurface();
	~CutawaySurface();

	void init(int w, int h);
	void prepareZBufferPass();
	void endZBufferPass();
	void prepareShadingPass(int unit);
	
	ZBufferShader* z_buffer_shader;
private:
	GLuint depth_map, fb;
	int width, height;
};