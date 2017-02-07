#include "ImageData.h"

ImageData::ImageData(string filename) {
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;

	FILE * file = fopen(filename.c_str(), "rb");
	if(!file) {
		cout << "File doesn't exist !!!\n";
		return ;
	}

	if(fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return ;
	}
	if(header[0] != 'B' || header[1] != 'M'){
		printf("Not a correct BMP file\n");
		return ;
	}
	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if(imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if(dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);
}
void ImageData::applyTexture() {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
}