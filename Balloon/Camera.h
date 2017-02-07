#pragma once 

#include "Includes.h"
#include "InputManager.h"
#include "Balloon.h"
enum CAMERA_TYPE{
	FREE_ROAM,
	BALLOON,
	HOVERING
};

class Camera {
public:
	glm::vec3 position;
	glm::vec3 rotation;

	float hoverDistance = 5.0f;
	glutil::MatrixStack camMatrix;

	Balloon* balloon;
	CAMERA_TYPE cameraType = HOVERING;

	void update(const InputManager& inputManager);
	void setMatrixUBO(GLuint globalMatrixUBO) { this->globalMatrixUBO = globalMatrixUBO ; }
	void setBalloon(Balloon* balloon) { this->balloon = balloon; }
private:
	glm::vec3 offset;
	GLuint globalMatrixUBO;

	void updateNonHovering(const InputManager& inputManager);
	void updateHovering(const InputManager& inputManager);

};