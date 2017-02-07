#include "Command.h"
#include "Game.h"

bool MoveCommand::update() {
	glm::vec3 distVector = dst - src;
	
	float distance = glm::length(distVector);
	glm::vec3 direction = glm::normalize(distVector);

	//cout << direction.x << " " << direction.y << " " << direction.z << endl;
	//cout << currSpeed << endl;
	glm::vec3 deltaVec = direction * (currSpeed * (float)Game::lastDelta);
	
	//cout << "Distance: " << distance << " DistanceGone: " << currSpeed * (float)Game::lastDelta << endl;

	if((currSpeed * (float)Game::lastDelta) - distance >= 0) {
		valueToToggle = !valueToToggle;
		src.x = dst.x;
		src.y = dst.y;
		src.z = dst.z;

		return true;
	}
	//cout << deltaVec.x << " " << deltaVec.y << " " << deltaVec.z << endl;
	//cout << src.x << " " << src.y << " " << src.z << endl;
	src += deltaVec;

	currSpeed += acc * Game::lastDelta;
	if(currSpeed > maxSpeed) currSpeed = maxSpeed;
	/*cout << src.x << " " << src.y << " " << src.z << endl;
	cout << endl;*/

	return false;
}
GrowTerrainCommand::GrowTerrainCommand(Ground* ground, long int timeToComplete, long int waitTime) : 
ground(ground), timeToComplete(timeToComplete), currTime(0), waitTime(waitTime) {
	verticesVec = &ground->verticesVec;
	rawData = ground->rawData;
	bufferObject = ground->groundBufferObject;
	
	orgVertices.resize(verticesVec->size());
	memcpy(&orgVertices[0], &(ground->verticesVec[0]), orgVertices.size() * sizeof(glm::vec4));
}
bool GrowTerrainCommand::update() {
	static bool firstTimeToggle = false;
	if(!firstTimeToggle) {
		firstTimeToggle = true;
		return false;
	}

	if(waitTime > 0) {
		waitTime -= Game::lastDelta;
		for(int i = 0; i < orgVertices.size(); i++)
			(*verticesVec)[i].y = 0;
		copyToMemory();
		return false;
	}

	currTime += Game::lastDelta;
	float perc = (float)currTime / timeToComplete;
	if(perc >= 1.0f) perc = 1.0f;

	for(int i = 0; i < orgVertices.size(); i++) 
		(*verticesVec)[i].y = orgVertices[i].y * perc;
	copyToMemory();

	if(perc == 1.0f) return true;
	return false;
}
void GrowTerrainCommand::copyToMemory() {
	memcpy(&rawData[0], &(*verticesVec)[0], verticesVec->size() * sizeof(glm::vec4));
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verticesVec->size() * sizeof(glm::vec4), &rawData[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}