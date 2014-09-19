#include "main.h"

SDL_Window* displayWindow;

Entity ball;
Entity paddle1;
Entity paddle2;

using namespace std;

GLuint LoadTexture(const char *image_path, GLuint format) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

void Setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

bool ProcessEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_UP]) {
		paddle1.direction_y = 1.0;
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		paddle1.direction_y = -1.0;
	}
	return true;
}

void Update(float elapsed) {
	paddle1.y += paddle1.direction_y * elapsed;
}

void Render() {
	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	paddle1.Draw();

	SDL_GL_SwapWindow(displayWindow);
}

void Cleanup() {

}



int main(int argc, char *argv[]) {
	
	Setup();

	float lastFrameTicks = 0.0f;

	paddle1.textureID = LoadTexture("emoji.png", GL_RGBA);
	paddle1.SetSize(0.1, 0.1);

	while (ProcessEvents()) {

		float ticks = (float)SDL_GetTicks()/1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		Update(elapsed);

		Render();
	}

	Cleanup();

	SDL_Quit();
	return 0;
}