#pragma once 

#include "Includes.h"
#include "Vector3f.h"

class Object{
public:
	vector<Vector3f> geometry;
	GLuint bufferObject;
	GLuint groundVAO;

	void render();
private:
	
};