#include "ShadowMap.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

using namespace glm;
using namespace std;

// Constructor usable without OpenGL context
CutawaySurface::CutawaySurface()
{
	
}
CutawaySurface::~CutawaySurface() {
	if (depth_map) {
		glDeleteTextures(1, &depth_map);
	}
	if (fb) {
		glDeleteBuffers(1, &fb);
	}
	if (z_buffer_shader) {
		delete z_buffer_shader; 
		z_buffer_shader = nullptr;
	}
}

// Initialize with OpenGL context
void CutawaySurface::init(int w, int h) {

	width = w;
	height = h;

	// Init depth map
	glGenTextures(1, &depth_map);
	glBindTexture(GL_TEXTURE_2D, depth_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	// Init framebuffer and attach depth map
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depth_map, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// Init z buffer shader
	z_buffer_shader = new ZBufferShader();
}

void CutawaySurface::prepareZBufferPass() {
	glDepthFunc(GL_GREATER);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}
void CutawaySurface::endZBufferPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CutawaySurface::prepareShadingPass(int unit) {
	glActiveTexture(GL_TEXTURE0 + unit); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_map);		// Shadow mapping with cube map
}
