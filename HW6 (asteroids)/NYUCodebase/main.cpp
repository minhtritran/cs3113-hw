
#include "Asteroids.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	Asteroids app;
	while (!app.UpdateAndRender()) {}
	return 0;
}