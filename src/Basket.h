#pragma once 

#include "Includes.h"

class Basket{
public:
	void construct(float balloonRadius, float lowestHeight);
	void render(int renderType);
private:
	GLuint bufferObject;
	GLuint lineIndex;

	GLuint addLineIndex;
	GLuint circleLineIndex;

	GLuint triangleIndexBuffer;
	GLuint revTriangleIndexBuffer;

	GLuint vao;

	int arraySize;
	int numbOfVertices; 
	int numbOfLines;
	int numbOfCircleSegments;
	int numbOfTriangles;
};