#pragma once 

#include "Includes.h"


class TestObject{
public:
	void construct();
	void render();
private:
	int numbOfVertices;
	int numbOfTriangles;

	GLuint vectorBufferObject;
	GLuint triangleIndexBuffer;
	GLuint vao;


};