#pragma once 

#include "Includes.h"

class Triangle{
public:
	glm::vec3 v1, v2, v3;

	Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) : v1(v1), v2(v2), v3(v3){}
	Triangle(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3) : v1(v1), v2(v2), v3(v3){}

	glm::vec3 calculateNormal(){
		glm::vec3 normal = glm::cross(v1 - v2, v3 - v2);
		return normal;
	}
};
