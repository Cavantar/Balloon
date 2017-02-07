#include "Game.h"
#include "Triangle.h"
#include "TerrainGenerator.h"


unsigned int Game::lastDelta = 0;

Game::Game() {
	srand(((unsigned int)time(NULL)));

	// Window Things 
	// ---
	mainWindowSize = fullHD ? Vector2i(1280, 720) : Vector2i(1000, 600);
	initializeWindow(mainWindowSize, "Balloon");

	//editWindow.initializeWindow(Vector2i(((fullHD ? 1920 : 1280) - 30) - mainWindowSize.x, mainWindowSize.y), "Editor Window", Vector2i(mainWindowSize.x + 10, 0));
	HWND consoleWindow = GetConsoleWindow();
	MoveWindow(consoleWindow, 0, mainWindowSize.y + 30, 1000, (fullHD ? 1080 : 720) - (mainWindowSize.y + 30), true);

	glutSetWindow(windowHandle);
	glutPopWindow();
	// ---

	loadShaders();
	
	setPerspMatrices();
	

	camera.setBalloon(&balloon);
	camera.setMatrixUBO(globalMatrixUBO);

	// Flag Stuff
	// ---
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	

	// --

	//AntTweakBar
	//--
	
	setTweakBar();

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
		);

	// Konstrukcja geometrii
	// --
	
	int tempTime = glutGet(GLUT_ELAPSED_TIME);

	balloon.construct();
	
	testObject.construct();
	
	ground.construct();
	
	//skyBox.construct();

	cout << "Time To Generate: " << float((glutGet(GLUT_ELAPSED_TIME) - tempTime)) / 1000.0f << " [s]" << endl;

	balloon.position.y = ground.maxY + 2.0f;
	camera.position.y = ground.maxY + 2.0f;
	// --
}
void Game::start() {
	
	glutMainLoop();
	
}
void Game::myRenderFunction() {
	
	fpsUpdate();
	
	updateCommands();
	camera.update(inputManager);
	updateLight();
	updateOnInput();

	glutil::MatrixStack localMatrix;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//skyBox.render();

	glUseProgram(lightTestProgram);
	
	simulateWind();
	
	balloon.render(renderType);
	
	localMatrix.SetIdentity();
	
	glUseProgram(lightTestProgram);
	glUniformMatrix4fv(lightToWorldMatrix, 1, GL_FALSE, glm::value_ptr(localMatrix.Top()));
	
	ground.render(renderType);
	
	localMatrix.SetIdentity();
	localMatrix.Translate(0, 2.0f, 0);
	glUniformMatrix4fv(lightToWorldMatrix, 1, GL_FALSE, glm::value_ptr(localMatrix.Top()));
	
	//testObject.render();

	glUseProgram(0);

	TwDraw();
	

}
void Game::fpsUpdate() {
	static int fps = 0;

	static int time = 0;
	static int lastTime = glutGet(GLUT_ELAPSED_TIME);

	while(glutGet(GLUT_ELAPSED_TIME) - lastTime < 1);
	
	lastDelta = glutGet(GLUT_ELAPSED_TIME) - lastTime;
	time += lastDelta;
	lastTime = glutGet(GLUT_ELAPSED_TIME);
	if(time > 1000) {
		time = time % 1000;
		stringstream temp;
		temp << "Balloon " << fps;
		glutSetWindowTitle(temp.str().c_str());
		fps = 0;
	}
	fps++;
}
void Game::updateCommands() {
	auto it = commands.begin();
	while(it != commands.end()) {
		if((*it)->update())it = commands.erase(it);
		else it++;
	}
}
void Game::setTweakBar() {
	TwWindowSize(mainWindowSize.x, mainWindowSize.y);
	TwInit(TW_OPENGL_CORE, NULL);

	TwBar *myBar;
	myBar = TwNewBar("TweakBar");

	// Kierunek Wiatru
	TwAddVarRW(myBar, "WindDir", TW_TYPE_DIR3F, &windDirection,
			   " label='WindDirection' opened=true help='Change the Wind Direction.' group='Wind'");
	// Si³a Wiatru
	TwAddVarRW(myBar, "WindSpeed", TW_TYPE_FLOAT, &windSpeed,
			   " label='WindSpeed' min=-100 max=100 step=0.5 keyIncr='+' keyDecr='-' help='Increase/decrease the wind speed.' group='Wind'");
	// Ostroœæ
	TwAddVarRW(myBar, "TerrainSharpness", TW_TYPE_FLOAT, &TerrainGenerator::sharpness,
			   " label='TerrainSharpness' min=0 max=500 step=2.00 keyIncr='+' keyDecr='-' help='Increase/decrease the terrain sharpness.' group='Terrain Generation'");
	// D³ugoœæ elementu Siatki
	TwAddVarRW(myBar, "GridLength", TW_TYPE_FLOAT, &ground.gridLength,
			   " label='GridLength' min=0 max=200 step=0.2 keyIncr='+' keyDecr='-' help='Increase/decrease the Grid Length.' group='Terrain Generation'");
	// D³ugoœæ siatki
	TwAddVarRW(myBar, "GridSize", TW_TYPE_INT32, &ground.desiredSideLength,
			   " label='GridSize' min=0 max=10 step=1 keyIncr='+' keyDecr='-' help='Increase/decrease the GridSize.' group='Terrain Generation'");
	// Przycisk do generowania
	TwAddVarRW(myBar, "Generate", TW_TYPE_BOOLCPP, &shouldGenerateTerrain,
			   " label='Generate' help='Generate.' group='Terrain Generation'");
	// Light
	TwAddVarRW(myBar, "DayLength", TW_TYPE_INT32, &timeToCompleteRound,
			   " label='DayLength' min=1000 max=100000000 step=100 keyIncr='+' keyDecr='-' help='Increase/decrease the DayLength.' group='Lighting'");
	// D³ugoœæ elementu Siatki
	TwAddVarRW(myBar, "CameraType", TW_TYPE_INT32, &camera.cameraType,
			   " label='CameraType' min=0 max=2 step=1 keyIncr='+' keyDecr='-' help='changeCamera.' ");
	TwAddVarRW(myBar, "GrowTime", TW_TYPE_INT32, &defaultGrowTime,
			   " label='GrowTime' min=0 max=10000000000 step=100 keyIncr='+' keyDecr='-' help='GrowTime' ");

	// ---
}
void Game::loadShaders() {
	vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "Shaders\\regular.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "Shaders\\fragment.frag"));

	programHandle = CreateProgram(shaderList);

	shaderList.clear();

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "Shaders\\light.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "Shaders\\fragment.frag"));
	
	lightTestProgram = CreateProgram(shaderList);
}
void Game::setPerspMatrices() {
	// Tworzenie bloku uniform'a odpowiedzialne za podstawowe macierze

	globalUniformBlockIndex = glGetUniformBlockIndex(programHandle, "GlobalMatrices");

	glGenBuffers(1, &globalMatrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, globalMatrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2 + sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	// Inicjalizacja macierzy perpektywy

	float fzNear = 0.5f;
	float fzFar = 1000.0f;

	glutil::MatrixStack persMatrix;
	persMatrix.Perspective(45.0f, mainWindowSize.x / (float)mainWindowSize.y, fzNear, fzFar);

	glBindBuffer(GL_UNIFORM_BUFFER, globalMatrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(persMatrix.Top()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Przyporz¹dkowanie bloku uniform'u do elementu w pamiêci o indeksie 1 

	glUniformBlockBinding(programHandle, globalUniformBlockIndex, 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, globalMatrixUBO, 0, sizeof(glm::mat4) * 2 + sizeof(glm::vec3));

	glUniformBlockBinding(lightTestProgram, globalUniformBlockIndex, 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, globalMatrixUBO, 0, sizeof(glm::mat4) * 2 + sizeof(glm::vec3));

	localToWorldMatrix = glGetUniformLocation(programHandle, "localToWorldMatrix");
	lightToWorldMatrix = glGetUniformLocation(lightTestProgram, "localToWorldMatrix");
}
void Game::updateLight() {
	glm::mat3 normalMatrix(camera.camMatrix.Top());
	glUseProgram(lightTestProgram);

	//Kierunek Œwiat³a
	// ------

	static float lightAngle = 3.14f / 3.0f;
	if(inputManager.isKeyDown('u')) lightAngle += 0.001f * lastDelta;

	lightDirection = glm::vec4(0, cos(lightAngle), sin(lightAngle), 0);
	static int localTime = 0;

	if(inputManager.isKeyDown('y')) localTime += lastDelta * 10;
	else localTime += lastDelta;
	localTime = localTime % timeToCompleteRound ;

	float radius = lightDirection.z;

	glm::vec4 tempVector;

	float trig1 = sin(((2 * 3.14f) / (float)timeToCompleteRound) * (float)localTime);
	float trig2 = cos(((2 * 3.14f) / (float)timeToCompleteRound) * (float)localTime);

	tempVector.x = lightDirection.x * trig2 - lightDirection.z *trig1;
	tempVector.y = lightDirection.y;
	tempVector.z = lightDirection.x * trig1 + lightDirection.z *trig2;

	// ------

	glm::vec4 lightDirCameraSpace = camera.camMatrix.Top() * tempVector;

	glBindBuffer(GL_UNIFORM_BUFFER, globalMatrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::vec3), glm::value_ptr(lightDirCameraSpace));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glUseProgram(0);
}
void Game::updateOnInput() {
	// Debug
	// -----
	if(inputManager.isKeyPressed('t') && ground.isColliding(camera.position)) cout << "Colliding !\n";

	// ----

	// Render Type Selection
	// ----
	if(inputManager.isKeyPressed('e')) renderType++;
	if(inputManager.isKeyPressed('q')) renderType--;

	if(renderType < 0) renderType = numbOfRenderTypes - 1;
	renderType = renderType % numbOfRenderTypes;

	// ---- 

	// Terrain Stuff
	// ----


	if(inputManager.isKeyPressed('7')) {
		commands.push_back(CommandPtr(new GrowTerrainCommand(&ground, defaultGrowTime, defaultGrowWaitTime)));
	}
	if(inputManager.isKeyPressed('1')) {
		TerrainGenerator::sharpness -= 10;
		cout << "TerrainSharpness: " << TerrainGenerator::sharpness << endl;
	}
	if(inputManager.isKeyPressed('2')) {
		TerrainGenerator::sharpness += 10;
		cout << "TerrainSharpness: " << TerrainGenerator::sharpness << endl;
	}
	if(inputManager.isKeyPressed('5')) {
		ground.gridLength -= 0.25f;
		cout << "GridLength: " << ground.gridLength << endl;
	}
	if(inputManager.isKeyPressed('6')) {
		ground.gridLength += 0.25f;
		cout << "GridLength: " << ground.gridLength << endl;
	}

	if(inputManager.isKeyPressed('r') || shouldGenerateTerrain) {
		int tempTime = glutGet(GLUT_ELAPSED_TIME);
		// Je¿eli wymiary siê ró¿ni¹ to trzeba wygenerowaæ kilka dodatkowych danych
		if(ground.desiredSideLength != ground.sideLength) {
			//cout << ground.sideLength << endl;
			ground.sideLength = ground.desiredSideLength;
			ground.construct();
		}
		else ground.reConstruct();
		commands.push_back(CommandPtr(new GrowTerrainCommand(&ground, defaultGrowTime, defaultGrowWaitTime)));

		balloon.position.y = ground.maxY + 2.0f;
		camera.position.y = ground.maxY + 2.0f;

		shouldGenerateTerrain = false;
		cout << "Time To Generate: " << float((glutGet(GLUT_ELAPSED_TIME) - tempTime)) / 1000.0f << " [s]" << endl;
	}

	// Camera Stuff
	// ----

	if(inputManager.isButtonPressed(3)) camera.hoverDistance -= 0.2f;
	if(inputManager.isButtonPressed(4)) camera.hoverDistance += 0.2f;


	static bool toggleValue = false;
	static bool balloonCamera = false;

	if(balloonCamera) {
		balloonCamera = false;
		camera.cameraType = BALLOON;
	}

	// Droga do balonu
	if(inputManager.isKeyPressed('4'))
		commands.push_back(CommandPtr(new MoveCommand(camera.position, balloon.position, 0.00002f, 10.0f, balloonCamera)));

	// Skok z balonu
	static glm::vec3 tempPosition;
	if(camera.cameraType == BALLOON && inputManager.isKeyPressed('3')){
		camera.cameraType = FREE_ROAM;
		toggleValue = false;
		camera.position = balloon.position - glm::vec3(0, 1.5f, 0);

		tempPosition = camera.position ;
		tempPosition.y = ground.getHighestPoint(camera.position);
		commands.push_back(CommandPtr(new MoveCommand(camera.position, tempPosition, 0.00002f, 100.0f, toggleValue)));
	}

	// Skok siê zakoñczy³
	if(toggleValue) {
		commands.push_back(CommandPtr(new MoveCommand(camera.position, balloon.position, 0.00002f, 100.0f, balloonCamera)));
		toggleValue = false;
	}

	// ----

}
void Game::simulateWind() {
	glutil::MatrixStack localMatrix;

	static float angleY = 0;
	static float angleX = 0;

	angleY += lastDelta * 0.01f;
	angleX += lastDelta * 0.05f;

	// Symulacja wiatru

	//phiAngle = acos(windDirection.x);
	//psiAngle = asin(windDirection.y);
	
	if(inputManager.isKeyDown('l')) phiAngle += 0.001f * lastDelta;
	if(inputManager.isKeyDown('k')) phiAngle -= 0.001f * lastDelta;

	if(inputManager.isKeyDown('o')) psiAngle += 0.001f * lastDelta;
	if(inputManager.isKeyDown('p')) psiAngle -= 0.001f * lastDelta;

	if(inputManager.isKeyDown('m')) windSpeed += 0.005f * lastDelta;
	if(inputManager.isKeyDown('n')) windSpeed -= 0.005f * lastDelta;



	//phiAngle = fmodf(phiAngle, 2 * 3.14);
	//psiAngle = fmodf(psiAngle, 2 * 3.14);

	//windDirection = glm::vec3(cos(phiAngle), sin(psiAngle), sin(phiAngle));

	glm::vec3 windDelta = windDirection;
	//windDelta.y *= -1.0f;
	windDelta *= windSpeed/1000.0f;
	
	balloon.position += windDelta;
	if(ground.isColliding(balloon.position, 0.8f)) balloon.position -= windDelta;

	localMatrix.Translate(balloon.position);
	localMatrix.RotateY(-(phiAngle / 3.14f) * 180.0f);
	//localMatrix.RotateX(angleX);
	
	glUniformMatrix4fv(lightToWorldMatrix, 1, GL_FALSE, glm::value_ptr(localMatrix.Top()));
	
}