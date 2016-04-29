// as seen in CG Tutorial (http://cg.tuwien.ac.at/courses/CG23/videos/2015/CG_Tut_2_part4%20-%20WLAN.m4v)

#pragma once

#include <string>
#include "glew.h"
#include "glfw3.h"
#include "../Scene/PointLight.h"
#include <vector>
#include <memory>

class Shader
{
public:
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	Shader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);
	~Shader();

	void useShader() const;
	virtual void setLighting(std::vector<std::shared_ptr<PointLight>> *allLights);
	virtual void bindTexture(int unit);

	GLuint programHandle;

private:
	void loadShader(const std::string& shader, GLenum shaderType, GLuint& handle);
	void link();
	void linkWithGeometryShader();

	GLuint vertexHandle;
	GLuint fragmentHandle;
	GLuint geometryHandle;
};