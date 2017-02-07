#include "Camera.h"
#include "Game.h"

void Camera::update(const InputManager& inputManager) {

	if(inputManager.isKeyPressed('c')) {
		cameraType = static_cast<CAMERA_TYPE>((cameraType + 1) % 3);
		
		/*if(cameraType == BALLOON) cameraType = FREE_ROAM;
		else cameraType = BALLOON;*/
	}

	if(inputManager.isButtonDown(GLUT_LEFT_BUTTON)) {
		rotation.y += inputManager.getMouseDelta().x * 0.2f;
		rotation.x += inputManager.getMouseDelta().y * 0.2f;
	}


	camMatrix.SetIdentity();

	static const float speedBoost = 5.0f;

	offset = glm::vec3();

	if(inputManager.isKeyDown('a')) offset.x = 0.001f * Game::lastDelta;
	if(inputManager.isKeyDown('d')) offset.x = -0.001f * Game::lastDelta;

	if(inputManager.isKeyDown('w')) offset.z = 0.001f * Game::lastDelta;
	if(inputManager.isKeyDown('s')) offset.z = -0.001f * Game::lastDelta;

	if(inputManager.isKeyDown('A')) offset.x = 0.001f * Game::lastDelta * speedBoost ;
	if(inputManager.isKeyDown('D')) offset.x = -0.001f * Game::lastDelta * speedBoost ;

	if(inputManager.isKeyDown('W')) offset.z = 0.001f * Game::lastDelta * speedBoost ;
	if(inputManager.isKeyDown('S')) offset.z = -0.001f * Game::lastDelta * speedBoost ;

	// Universal Stuff Up To This Point

	if(cameraType != HOVERING) updateNonHovering(inputManager);
	else updateHovering(inputManager);

	glBindBuffer(GL_UNIFORM_BUFFER, globalMatrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camMatrix.Top()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Camera::updateNonHovering(const InputManager& inputManager) {
	camMatrix.RotateX(rotation.x);

	if(cameraType == FREE_ROAM) camMatrix.RotateY(rotation.y);
	if(cameraType == BALLOON) camMatrix.RotateY(rotation.y + ((balloon->phiAngle / (3.14f)) * 180.0f));

	//else camMatrix.RotateY(rotation.y + ((balloon->phiAngle / (3.14f)) * 180.0f));

	camMatrix.RotateY(rotation.y);

	const glm::mat4& temp = camMatrix.Top();

	glm::vec3 rightVector = glm::vec3(temp[0].x, temp[0].y, temp[0].z);
	glm::vec3 upVector = glm::vec3(temp[1].x, temp[1].y, temp[1].z);
	glm::vec3 lookVector = glm::vec3(temp[2].x, temp[2].y, temp[2].z);

	if(cameraType == FREE_ROAM) camMatrix.Translate(-position);
	if(cameraType == BALLOON) camMatrix.Translate(-(balloon->position - glm::vec3(0, 1.5f, 0)));

	glm::vec3 tempPosition(offset.x, 0, offset.z);

	position.x -= dotProduct(tempPosition, rightVector);
	position.y -= dotProduct(tempPosition, upVector);
	position.z -= dotProduct(tempPosition, lookVector);

}
void Camera::updateHovering(const InputManager& inputManager) {

	camMatrix.Translate(glm::vec3(0, 0, -hoverDistance));
	camMatrix.Rotate(glm::vec3(1.0f, 0, 0), rotation.x);
	camMatrix.Rotate(glm::vec3(0, 1.0f, 0),rotation.y);
	

	camMatrix.Translate(-(balloon->position - glm::vec3(0, 1.5f, 0)));
}