#pragma once 

#include "Window.h"
#include "EditorWindow.h"
#include "Balloon.h"
#include "Ground.h"
#include "TestObject.h"
#include "Command.h"
#include "Camera.h"
#include "SkyBox.h"

class Game : public Window{
public:
	Game();
	void start();

	static unsigned int lastDelta;
private:
	const bool fullHD = true;
	Vector2i mainWindowSize;

	int renderType = 1;
	const int numbOfRenderTypes = 3;

	EditorWindow editWindow;
	
	//Uchwyty
	GLuint programHandle;

	GLuint vao;

	GLuint globalMatrixUBO;
	GLuint localToWorldMatrix;

	GLuint cubeBufferObject;
	GLuint indexBufferObject;

	// Light Stuff

	GLuint lightTestProgram;
	GLuint lightToWorldMatrix;
	GLuint normalModelToCameraMatrixUnif;
	GLuint lightDirectionUnif;

	int timeToCompleteRound = 100000;

	glm::vec4 lightDirection = glm::vec4(0, 0, 1.0f, 0);

	// Objects

	SkyBox skyBox;
	Balloon balloon;
	Ground ground;
	TestObject testObject;

	bool shouldGenerateTerrain = false;

	// Wiatr

	float phiAngle = 0;
	float psiAngle = 0;

	float windSpeed = 0;

	glm::vec3 windDirection = glm::vec3(1.0f, 0, 0);

	void simulateWind();

	// Kamera

	Camera camera;

	// Komendy

	list<CommandPtr> commands;
	int defaultGrowTime = 2500;
	int defaultGrowWaitTime = 2000;

	//Uniforms

	GLuint globalUniformBlockIndex;

	GLuint offsetUniform;
	GLuint perspectiveMatrixUnif;

	// Metody

	void myRenderFunction();
	void fpsUpdate();
	void updateCommands();
	void setTweakBar();

	void setPerspMatrices();
	void updateLight();
	void updateOnInput();

	void loadShaders();
};