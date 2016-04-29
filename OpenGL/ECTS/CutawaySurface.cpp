#include "CutawaySurface.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

using namespace glm;
using namespace std;

// Constructor usable without OpenGL context
CutawaySurface::CutawaySurface()
: last_target(1)
{
	
}
CutawaySurface::~CutawaySurface() {
	if (depth_map) {
		glDeleteTextures(1, &depth_map);
	}
	if (fbo1) {
		glDeleteBuffers(1, &fbo1);
	}
	if (z_buffer_shader) {
		delete z_buffer_shader; 
		z_buffer_shader = nullptr;
	}
	if (quad) {
		delete quad;
		quad = nullptr;
	}
	if (quad_shader) {
		delete quad_shader;
		quad_shader = nullptr;
	}
}

// Initialize with OpenGL context
void CutawaySurface::init(int w, int h, float z_near, float z_far, float angle) {

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
	glGenFramebuffers(1, &fbo1);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depth_map, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Init second texture and framebuffer for jump flooding
	glGenTextures(1, &target_tex);
	glBindTexture(GL_TEXTURE_2D, target_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenFramebuffers(1, &fbo2);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target_tex, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	// Init z buffer shader
	z_buffer_shader = new ZBufferShader();

	// Init quad + quad shader
	quad = new Quad();
	quad_shader = new QuadShader(z_near, z_far, angle);
}

void CutawaySurface::prepareZBufferPass() {
	glDepthFunc(GL_GREATER);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}
void CutawaySurface::endZBufferPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CutawaySurface::prepareQuadPass(int step, mat4& vp) {
	glViewport(0, 0, width, height);

	if (last_target == 2) {
		look_up_tex = temp_tex;
		target_fbo = fbo1;
		last_target = 1;
	}
	else {
		look_up_tex = depth_map;
		target_fbo = fbo2;
		last_target = 2;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, target_fbo);
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	quad_shader->useShader();

	// Set step length
	auto step_location = glGetUniformLocation(quad_shader->programHandle, "step");
	glUniform1i(step_location, step);

	// Set view projection matrix
	auto view_proj_location = glGetUniformLocation(quad_shader->programHandle, "view_proj");
	glUniformMatrix4fv(view_proj_location, 1, GL_FALSE, glm::value_ptr(vp));

	// Set model matrix
	auto model_location = glGetUniformLocation(quad_shader->programHandle, "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));


	quad->bindVAO();
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, look_up_tex);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}


void CutawaySurface::prepareShadingPass(int unit) {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
