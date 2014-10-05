
#include "PlatKillas.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	PlatKillas app;
	while (!app.UpdateAndRender()) {}
	return 0;
}