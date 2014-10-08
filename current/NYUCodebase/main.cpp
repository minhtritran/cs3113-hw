
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	return 0;
}

void ClassDemoApp::buildLevel() {
	deathLimit = 3;
	birthLimit = 4;
	srand(SDL_GetTicks());

	memset(levelData, 0, LEVEL_HEIGHT * LEVEL_WIDTH);

	for (int x = 0; x < LEVEL_WIDTH; x++) {
		for (int y = 0; y < LEVEL_HEIGHT; y++) {
			if (rand() % 10 < 6) {
				levelData[y][x] = 0;
			}
			else {
				levelData[y][x] = 32 + (rand() % 3);
			}
		}
	}

	for (int i = 0; i < 6; i++) {
		doSimulationStep();
	}
}

void ClassDemoApp::doSimulationStep() {
	unsigned char newData[LEVEL_HEIGHT][LEVEL_WIDTH];

	for int x level width{
		for y level height{
			int nbs = countAliveNeigbors(x, y);
			
			if (levelData[x][y] != 0) {
				if (nbs < deathLimit) {
					newData[x][y] = 0;
				}
				else {
					newData[x][y] = 32 + (rand() % 3);
				}
			}
		}
	}
		
}

int ClassDemoApp::countAliveNeigbors(float x, float aay) {

}