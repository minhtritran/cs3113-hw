
#include "SideScroller.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	SideScroller app;
	while (!app.UpdateAndRender()) {}
	return 0;
}