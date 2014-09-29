
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

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}

void ClassDemoApp::FixedUpdate() {
	for (int i = 0; i < entities.size(); i++) {
		entities[i].FixedUpdate();
		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}
		
	}

	for (int i = 0; i < entities.size(); i++) {

		//y axis

		entities[i]->y = += entities->velocity_y * FIXED_TIMESTEP;

		for (int j = 0; j < entities.size(); j++) {
			if (entities[j] != entities[i]) {
				if (entities[i]->collidesWith(*entities[j])) {
					float yPenetration = fabs(entities[j]->y - entities[i]->y) - entities[i]->height / 2.0f - entities[j]->height / 2.0f;

					if (entities[j]->y > entities[i]->y) {
						entities[j]->y -= yPenetration + 0.001f;
					}
					else {
						entities[j]->y += yPenetration + 0.001f;
					}
				}
			}
		}

		//x axis

		entities[i]->x = += entities->velocity_x * FIXED_TIMESTEP;

		for (int j = 0; j < entities.size(); j++) {
			if (entities[j] != entities[i]) {
				if (entities[i]->collidesWith(*entities[j])) {
					float xPenetration = fabs(entities[j]->x - entities[i]->x) - entities[i]->width / 2.0f - entities[j]->width / 2.0f;

					if (entities[j]->x > entities[i]->x) {
						entities[j]->x -= xPenetration + 0.001f;
					}
					else {
						entities[j]->x += xPenetration + 0.001f;
					}

					
				}
			}
		}
		
		

	}
}