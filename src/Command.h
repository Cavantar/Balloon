#pragma once 

#include "Includes.h"

class Ground;
class Command{
public:
	virtual ~Command(){};
	virtual bool update() = 0 ;
};

class MoveCommand : public Command {
public:
	MoveCommand(glm::vec3& src, glm::vec3& dst, float acc, float maxSpeed, bool& valueToToggle) : 
		src(src), dst(dst), acc(acc), maxSpeed(maxSpeed), valueToToggle(valueToToggle){
		currSpeed = 0.0001f;
	};
	bool update();
private:
	glm::vec3& src;
	glm::vec3& dst;

	bool& valueToToggle;
	
	float acc, maxSpeed;
	float currSpeed;

};
class GrowTerrainCommand : public Command {
public:
	GrowTerrainCommand(Ground* ground, long int timeToComplete, long int waitTime);
	bool update();
private:
	Ground* ground;
	vector<glm::vec4>* verticesVec;
	vector<float> rawData;
	GLuint bufferObject;

	vector<glm::vec4> orgVertices;
	long int timeToComplete;
	long int waitTime;
	long int currTime;

	void copyToMemory();
};

typedef shared_ptr<Command> CommandPtr;