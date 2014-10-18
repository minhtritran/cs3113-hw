
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;

	SDL_Event event;

	glViewport(0,0,800,600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(1.0, 0.0, 0.0);
		glLoadIdentity();
		glRotatef(45.0f, 0.0, 0.0, 1.0);
		glLoadIdentity();
		glScalef(2.0, 1.0, 1.0);

		GLfloat triangle[] = {0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f};
		glVertexPointer(2, GL_FLOAT, 0, triangle);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}