#include "main.h"

using namespace std;

SDL_Window* displayWindow;

Entity ball(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 2.0f, 1.0f, 1.0f, 1.0f);
Entity paddle1(-1.2f, 0.0f, 0.0f, 0.075f, 0.3f, 2.0f, 1.0f, 1.0f, 1.0f);
Entity paddle2(1.2f, 0.0f, 0.0f, 0.075f, 0.3f, 2.0f, 1.0f, 1.0f, 1.0f);
Entity wall_top(0.0f, 1.0f, 0.0f, 2.67f, 0.05f, 0.0f, 1.0f, 1.0f, 1.0f);
Entity wall_bottom(0.0f, -1.0f, 0.0f, 2.67f, 0.05f, 0.0f, 1.0f, 1.0f, 1.0f);

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

bool isColliding(const Entity& e1, const Entity& e2) {
	float e1_y_max = e1.y + e1.height / 2;
	float e1_y_min = e1.y - e1.height / 2;
	float e1_x_max = e1.x + e1.width / 2;
	float e1_x_min = e1.x - e1.width / 2;

	float e2_y_max = e2.y + e2.height / 2;
	float e2_y_min = e2.y - e2.height / 2;
	float e2_x_max = e2.x + e2.width / 2;
	float e2_x_min = e2.x - e2.width / 2;

	if (e1_y_min > e2_y_max)
		return false;
	if (e1_y_max < e2_y_min)
		return false;
	if (e1_x_min > e2_x_max)
		return false;
	if (e1_x_max < e2_x_min)
		return false;

	return true;
}

void Setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	ball.textureID = LoadTexture("diamond.png", GL_RGBA);
}

bool ProcessEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return false;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (ball.direction_x == 0 && ball.direction_y == 0) {
					ball.direction_x = ball.speed;
					ball.direction_y = ball.speed / 2;
				}
			}
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]) {
		paddle1.direction_y = paddle1.speed;
	}
	else if (keys[SDL_SCANCODE_S]) {
		paddle1.direction_y = -paddle1.speed;
	}
	if (keys[SDL_SCANCODE_UP]) {
		paddle2.direction_y = paddle2.speed;
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		paddle2.direction_y = -paddle2.speed;
	}

	return true;
}

void Update(float elapsed) {
	if (!(isColliding(paddle1, wall_top) && paddle1.direction_y > 0) && !(isColliding(paddle1, wall_bottom) && paddle1.direction_y < 0))
		paddle1.y += paddle1.direction_y * elapsed;
	if (!(isColliding(paddle2, wall_top) && paddle2.direction_y > 0) && !(isColliding(paddle2, wall_bottom) && paddle2.direction_y < 0))
		paddle2.y += paddle2.direction_y * elapsed;

	if ((isColliding(ball, paddle1) && ball.direction_x < 0) || isColliding(ball, paddle2) && ball.direction_x > 0)
		ball.direction_x = ball.direction_x * -1;

	if ((isColliding(ball, wall_top) && ball.direction_y > 0) || (isColliding(ball, wall_bottom) && ball.direction_y < 0))
		ball.direction_y = ball.direction_y * -1;
	
	ball.x += ball.direction_x * elapsed;
	ball.y += ball.direction_y * elapsed;

	//Player 1 wins!
	if (ball.x > 1.33) {
		ball.direction_x = 0;
		ball.direction_y = 0;
		ball.x = 0;
		ball.y = 0;
		if (paddle1.red == 1.0 && paddle1.green == 1.0 && paddle1.blue == 1.0)
			paddle1.setColor(0.0, 0.0, 0.0);
		else
			paddle1.setColor(1.0, 1.0, 1.0);
	}
	//Player 2 wins!
	else if (ball.x < -1.33) {
		ball.direction_x = 0;
		ball.direction_y = 0;
		ball.x = 0;
		ball.y = 0;
		if (paddle2.red == 1.0 && paddle2.green == 1.0 && paddle2.blue == 1.0)
			paddle2.setColor(0.0, 0.0, 0.0);
		else
			paddle2.setColor(1.0, 1.0, 1.0);
	}

	paddle1.direction_y = 0;
	paddle2.direction_y = 0;
}

void Render() {
	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	paddle1.Draw();
	paddle2.Draw();
	ball.Draw();
	wall_top.Draw();
	wall_bottom.Draw();

	SDL_GL_SwapWindow(displayWindow);
}

void Cleanup() {

}



int main(int argc, char *argv[]) {
	
	Setup();

	float lastFrameTicks = 0.0f;

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