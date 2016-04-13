#pragma once

#include "glew.h"
#include <string>

class Texture {
public:
	Texture(const std::string& path);
	~Texture();

	void bind(int unit);

private:
	unsigned char* loadFile(const char* path);
	GLuint handle;
	unsigned char* data;
	int width, height;

};