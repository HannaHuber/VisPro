// as seen in CG Tutorial (http://cg.tuwien.ac.at/courses/CG23/videos/2015/CG_Tut_2_part4%20-%20WLAN.m4v)
#include "Shader.h"

#include <iostream>
#include <fstream>

using namespace std;

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
: programHandle(0)
,vertexHandle(0)
,fragmentHandle(0)
{
	programHandle = glCreateProgram();

	if (programHandle == 0)
	{
		std::cout << "Error in Shader.cpp: programHandle was not initialised with glCreateProgram()" << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	loadShader(vertexShader, GL_VERTEX_SHADER, vertexHandle);
	loadShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentHandle);
	link();
}
Shader::~Shader()
{
	glDeleteProgram(programHandle);
	glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);
}

void Shader::useShader() const
{
	glUseProgram(programHandle);
}


void Shader::loadShader(const std::string& shader, GLenum shaderType, GLuint& handle)
{
	std::ifstream shaderFile(shader);
	
	if (!shaderFile.good())
	{
		std::cout << "Error in Shader.cpp: Loading file '" << shader << "' failed!" << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	std::string code = std::string(std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>());
	shaderFile.close();
	handle = glCreateShader(shaderType);

	if (handle == 0)
	{
		std::cout << "Error in Shader.cpp: Could not create Shader" << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	const char * codePtr = code.c_str();
	glShaderSource(handle, 1, &codePtr, nullptr);
	glCompileShader(handle);

	GLint succeeded;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &succeeded);
	if (!succeeded){
		GLint logSize;
		glGetShaderiv(handle,GL_INFO_LOG_LENGTH,&logSize);
		GLchar* msg = new GLchar[logSize];
		glGetShaderInfoLog(handle, logSize, nullptr, msg);

		std::cout << msg << std::endl;
		system("PAUSE");
		delete[] msg;
		exit(EXIT_FAILURE);
	}
}

void Shader::link()
{
	glAttachShader(programHandle, vertexHandle);
	glAttachShader(programHandle, fragmentHandle);

	// Set output
	glBindFragDataLocation(programHandle, 0, "outColor");

	glLinkProgram(programHandle);
	
	GLint succeeded;
	glGetProgramiv(programHandle, GL_COMPILE_STATUS, &succeeded);
	if (!succeeded){
		GLint logSize;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);
		GLchar* msg = new GLchar[logSize];
		glGetProgramInfoLog(programHandle, logSize, nullptr, msg);

		std::cout << msg << std::endl;
		system("PAUSE");
		delete[] msg;
		exit(EXIT_FAILURE);
	}
}

void Shader::setLighting(std::vector<std::shared_ptr<PointLight>> *allLights) {

}

void Shader::bindTexture(int unit) {

}