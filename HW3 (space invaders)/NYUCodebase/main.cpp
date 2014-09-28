
#include "SpaceInvaders.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	SpaceInvaders app;
	while (!app.UpdateAndRender()) {}
	return 0;
}