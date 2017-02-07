#pragma once 

class Vector2f{
public:
	float x;
	float y;

	Vector2f(const float x = 0, const float y = 0) : x(x), y(y) {}
};

bool glCheckErrors();
#define  GL_CHECK_ERRORS if(glCheckErrors()) assert(0);
