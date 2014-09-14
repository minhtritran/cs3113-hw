
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

void DrawSprite(GLint texture, float x, float y, float rotation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;
	
	SDL_Event event;

	GLuint diamondTexture = LoadTexture("element_blue_diamond.png");
	GLuint enemyTexture = LoadTexture("slimeWalk1.png");
	GLuint p1Texture = LoadTexture("p1_front.png");
	GLuint p2Texture = LoadTexture("p3_hurt.png");

	float lastFrameTicks = 0.0f;
	float faceRotation = 0.0;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		glClearColor(0.5f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glTranslatef(0.0, 0.0, 0.0);
		GLfloat triangle[] = { 0.7f, -0.1f, -0.7f, -0.1f, 0.0f, -0.7f };
		glVertexPointer(2, GL_FLOAT, 0, triangle);
		glEnableClientState(GL_VERTEX_ARRAY);
		GLfloat triangleColors[] = { 1.0, 1.0, 1.0, 0.0, 0.5, 0.0, 0.0, 0.0, 1.0 };
		glColorPointer(3, GL_FLOAT, 0, triangleColors);
		glEnableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableClientState(GL_COLOR_ARRAY);

		//Spinning diamond
		faceRotation += 45.0f * elapsed;
		DrawSprite(diamondTexture, 0.5, 0.5, faceRotation);

		DrawSprite(p1Texture, 0.0, 0.0, 0.0);
		DrawSprite(p2Texture, -0.4, 0.0, 0.0);
		DrawSprite(enemyTexture, 0.4, 0.0, 0.0);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}