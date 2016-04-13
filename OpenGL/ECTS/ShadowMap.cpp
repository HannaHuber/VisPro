#include "ShadowMap.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

using namespace glm;
using namespace std;

// Constructor usable without OpenGL context
ShadowMap::ShadowMap(int ww, int hh, float fov, float near, float far)
: width(ww)
, height(hh)
, planes(vec2(near, far))
, proj(glm::perspective(glm::radians(fov), ww / (float)hh, near, far))
{
	
}
ShadowMap::~ShadowMap() {
	if (depth_cube) {
		glDeleteTextures(1, &depth_cube);
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
void ShadowMap::init() {
	// Init cube map
	glGenTextures(1, &depth_cube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cube);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // TODO Use linear/nn ?
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE); // Needed for PCF
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	// Faces/directions defined by i: 
	// 0=+x, 1=-x, 2=+y, 3=-y, 4=+z, 5=-z
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0); 
	}

	// Init framebuffer and attach zcube
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_cube, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Init z buffer shader + uniforms
	z_buffer_shader = new ZBufferShader();
	z_buffer_shader->useShader();
	auto far_location = glGetUniformLocation(z_buffer_shader->programHandle, "sm_planes");
	glUniform2fv(far_location, 1, glm::value_ptr(planes));
	auto proj_location = glGetUniformLocation(z_buffer_shader->programHandle, "proj");
	glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj));
	glUseProgram(0);

}

void ShadowMap::prepareZBufferPass() {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glClear(GL_DEPTH_BUFFER_BIT); 
	//glEnable(GL_POLYGON_OFFSET_FILL);		// Avoid shadow acne manually in ZBufferShader.frag
	//glPolygonOffset(4.f, 0.f);
}
void ShadowMap::endZBufferPass() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_POLYGON_OFFSET_FILL);
}

void ShadowMap::prepareShadingPass(int unit) {
	glActiveTexture(GL_TEXTURE0 + unit); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cube);		// Shadow mapping with cube map
}

void ShadowMap::setLight(shared_ptr<PointLight> l, int id) {
	// Init light
	light = l;
	light_id = id;
	vec3 light_pos = l->location();
	
	// Init view matrices
	view[0] = lookAt(light_pos, light_pos + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));		// Right
	view[1] = lookAt(light_pos, light_pos + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f));		// Left
	view[2] = lookAt(light_pos, light_pos + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));		// Up
	view[3] = lookAt(light_pos, light_pos + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f));		// Down
	view[4] = lookAt(light_pos, light_pos + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f));		// Front
	view[5] = lookAt(light_pos, light_pos + vec3(0.0f, 0.0, -1.0f), vec3(0.0f, -1.0f, 0.0f));		// Back
	
	// Set shader uniforms
	z_buffer_shader->useShader();
	auto light_location = glGetUniformLocation(z_buffer_shader->programHandle, "light_pos");
	glUniform3fv(light_location, 1, glm::value_ptr(light_pos));
	auto view_location = glGetUniformLocation(z_buffer_shader->programHandle, "view[0]");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view[0]));
		view_location = glGetUniformLocation(z_buffer_shader->programHandle, "view[1]");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view[1]));
		view_location = glGetUniformLocation(z_buffer_shader->programHandle, "view[2]");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view[2]));
		view_location = glGetUniformLocation(z_buffer_shader->programHandle, "view[3]");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view[3]));
		view_location = glGetUniformLocation(z_buffer_shader->programHandle, "view[4]");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view[4]));
		view_location = glGetUniformLocation(z_buffer_shader->programHandle, "view[5]");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view[5]));
	glUseProgram(0);
}