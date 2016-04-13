#include "Texture.h"
#include "FreeImage.h"
#include <iostream>

using namespace std;

Texture::Texture(const std::string& path) {
	unsigned char* data = loadFile(path.c_str());														// Get image converted to 24 bits
	glGenTextures(1, &handle);																	// Generate texture
	glActiveTexture(GL_TEXTURE0);																// Bind texture
	glBindTexture(GL_TEXTURE_2D, handle);											
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);	// Upload data to GPU
	glGenerateMipmap(GL_TEXTURE_2D);															// Generate mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);				// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);									

}
Texture::~Texture() {
	glDeleteTextures(1, &handle);
}

unsigned char* Texture::loadFile(const char* path) {
	// Get file format
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path, 0);
	if (format == FIF_UNKNOWN) {
		cout << "Error: Unknown file format for path:" << path << endl;
		system("PAUSE");
		exit(-1);
	}

	// Load image
	FIBITMAP* image = FreeImage_Load(format, path);
	if (!image) {
		cout << "Error: Could not load image file" << endl;
		system("PAUSE");
		exit(-1);
	}

	// Convert to 24 bits to use GL_RGB
	FIBITMAP* image_bits = FreeImage_ConvertTo24Bits(image);
	if (!image_bits) {
		cout << "Error: Could convert image to 24 bits" << endl;
		system("PAUSE");
		exit(-1);
	}

	// Get image dimensions
	width = FreeImage_GetWidth(image_bits);				
	height = FreeImage_GetHeight(image_bits);

	// Unload image
	FreeImage_Unload(image);

	// Return bits
	return (unsigned char*)FreeImage_GetBits(image_bits);	
}
void Texture::bind(int unit) {
	glActiveTexture(GL_TEXTURE0	+ unit);
	glBindTexture(GL_TEXTURE_2D,handle);
}