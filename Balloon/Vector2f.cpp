#include "Vector2f.h"
#include "Includes.h"

bool glCheckErrors() {
	GLenum error = glGetError();
	int i = 0;
	if(error != GL_NO_ERROR) {
		while(error != GL_NO_ERROR) {
			switch(error) {
				case GL_INVALID_ENUM:
					cout << "InvalidEnum\n";
					break;
				case GL_INVALID_VALUE:
					cout << "InvalidValue\n";
					break;
				case GL_INVALID_OPERATION:
					cout << "InvalidOperation\n";
					break;
				case GL_STACK_OVERFLOW:
					cout << "StackOverflow\n";
					break;
				case GL_STACK_UNDERFLOW:
					cout << "StackUnderOverflow\n";
					break;
				case GL_OUT_OF_MEMORY:
					cout << "OutOfMemory\n";
					break;
				case GL_TABLE_TOO_LARGE:
					cout << "TableTooLarge\n";
					break;
				default:
					cout << " WTFF ??? \n";
			}
			error = glGetError();
			return true;
		}
		return true;
	}
	return false;
}
