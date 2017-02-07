#include "Window.h"

void Window::initializeWindow(const Vector2i dimensions, const string caption, const Vector2i position) {
	GLenum GlewInitResult;

	//glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

	glutInitWindowSize(dimensions.x, dimensions.y);
	glutInitWindowPosition(position.x, position.y);

	//glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	windowHandle = glutCreateWindow(caption.c_str());

	glutSetWindowData((void *) this);

	if(windowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		exit(EXIT_FAILURE);
	}

	// Rejestracja funkcji callback

	glutReshapeFunc(resizeFunction);
	glutDisplayFunc(renderFunction);

	glutKeyboardFunc(keyboardFunction);
	glutKeyboardUpFunc(keyboardUpFunction);

	glutMouseFunc(mouseFunction);
	glutPassiveMotionFunc(mouseMove);
	glutMotionFunc(mouseMove);

	GlewInitResult = glewInit();

	if(GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
			);
		exit(EXIT_FAILURE);
	}
	if(windowHandle == 1) glClearColor(78.0f / 255.0f, 149.0f / 255.0f, 199.0f / 255.0f, 0.0f);
	else glClearColor(256.0f, 256.0f, 256.0f, 0.0f);


}

void Window::resizeFunction(int Width, int Height) {
	glViewport(0, 0, Width, Height);
}
void Window::renderFunction() {
	((Window*)glutGetWindowData())->myRenderFunction();
	((Window*)glutGetWindowData())->inputManager.clear();

	glutSwapBuffers();
	glutPostRedisplay();
}
void Window::keyboardFunction(unsigned char key, int x, int y) {
	if(key == 27) glutLeaveMainLoop();
	if(!TwEventKeyboardGLUT(key,x,y))
		((Window*)glutGetWindowData())->inputManager.handleKeyPress(key);
}
void Window::keyboardUpFunction(unsigned char key, int x, int y) {
	((Window*)glutGetWindowData())->inputManager.handleKeyRelease(key);
}

void Window::mouseFunction(int button, int state, int x, int y) {
	if(!TwEventMouseButtonGLUT(button, state, x, y)){
		if(state == GLUT_DOWN)
			((Window*)glutGetWindowData())->inputManager.handleButtonPress(button);
		else ((Window*)glutGetWindowData())->inputManager.handleButtonRelease(button);
	}
}
void Window::mouseMove(int x, int y) {
	if(!TwEventMouseMotionGLUT(x,y))  
		((Window*)glutGetWindowData())->inputManager.handleMouseMove(Vector2i(x, y));
}