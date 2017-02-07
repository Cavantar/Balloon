#pragma once 
#include "Includes.h"

class ImageData{
public:
	ImageData(string filename);
	~ImageData(){ delete[] data; }

	void applyTexture();
private:
	unsigned int width, height;
	unsigned char * data;
};