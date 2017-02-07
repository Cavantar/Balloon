#pragma once 

#include "Includes.h"
#include "Basket.h"

class Balloon {
public:
	void construct();
	void render(int renderType);

	glm::vec3 position = glm::vec3(0, 2, -10.0f);
	
	float phiAngle = 0;
	float psiAngle = 0;

	float windSpeed = 0;
private:
	Basket basket;

	GLuint balloonVAO;
	GLuint balloonBufferObject;
	GLuint triangleIndexBuffer;
	GLuint revTriangleIndexBuffer;

	GLuint balloonLineIndex;
	GLuint balloonNumberOfLineIndices;

	size_t numberOfIndices;

	int numberOfVertices;
	int numbOfTriangles;
};