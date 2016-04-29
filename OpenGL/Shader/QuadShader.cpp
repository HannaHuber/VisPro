#include "QuadShader.h"

QuadShader::QuadShader(float z_near, float z_far, float angle)
: Shader("./Shader/QuadShader.vert", "./Shader/QuadShader.frag")
{
	useShader();

	// Set projection matrix z-scaling factor
	auto pmsz_location = glGetUniformLocation(programHandle, "PMsz");
	glUniform1f(pmsz_location, (z_near+z_far)/(z_near-z_far));
	
	// Set tangent of drill angle
	auto tanphi_location = glGetUniformLocation(programHandle, "tanPhi");
	glUniform1f(tanphi_location, glm::tan(angle));

	glUseProgram(0);
}

QuadShader::~QuadShader() {

}