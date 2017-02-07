#pragma once 

#include "Includes.h"
#include "Vector2i.h"
#include "InputManager.h"

class Window {
public:
	unsigned int windowHandle;
	InputManager inputManager;

	void initializeWindow(const Vector2i dimensions, const string caption = "", const Vector2i position = Vector2i());
private:
	
	static void resizeFunction(int Width, int Height);
	static void renderFunction();
	
	static void keyboardFunction(unsigned char key, int x, int y);
	static void keyboardUpFunction(unsigned char key, int x, int y);

	static void mouseFunction(int button, int state, int x, int y);

	static void mouseMove(int x, int y);

	virtual void myRenderFunction() = 0;
};