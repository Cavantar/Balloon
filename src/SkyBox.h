#pragma once 

#include "Includes.h"

class SkyBox{
public:
	void construct();
	void render();
	void setPerspMat(glutil::MatrixStack matrix);

private:
	// Shaders 
	GLuint skyBoxProgram;
	GLuint uProjectionMatrix;
	GLuint uWorldToCameraMatrix;

	GLuint textureId;
	GLuint cubeTextureId;

	GLuint triangleIndexBuffer;
	GLuint verticesBuffer;

	GLuint vao;


	void doShaderInit();
	void createUniforms();
	void loadTexture(string filename);

};