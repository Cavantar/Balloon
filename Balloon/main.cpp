#include "Includes.h"
#include "Game.h"
#include "TerrainGenerator.h"

int main(int argc, char* argv[]) {
	bool test = false;
	if(test) {
		//TerrainGenerator::generateTerrain(3, glm::vec4(1, 2, 3, 3));
		loadBMP_custom("Gfx\\bitmap.bmp");
	}
	else {
		glutInit(&argc, argv);
		Game game;
		game.start();
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	exit(EXIT_SUCCESS);
}