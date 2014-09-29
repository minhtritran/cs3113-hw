
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(surface);

	return textureID;
}

class SheetSprite {
	public:
		SheetSprite();
		SheetSprite(unsigned int textureID, float u, float v, float width, float height);

		void Draw(float scale);

		unsigned int textureID;
		float u;
		float v;
		float width;
		float height;
};

SheetSprite::SheetSprite() {
}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height)
	: textureID(textureID), u(u), v(v), width(width), height(height) {}

void SheetSprite::Draw(float scale) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLfloat quad[] = { scale*-width, scale*height, scale*-width, scale*-height, scale*width, scale*-height, scale*width, scale*height };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat uvs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableClientState(GL_COLOR_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;
	
	SDL_Event event;

	unsigned int spriteSheetTexture = LoadTexture("sheet.png");

	//name="enemyBlue1.png" x="425" y="468" width="93" height="84" />

	SheetSprite mySprite = SheetSprite(spriteSheetTexture, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f);

	glViewport(0,0,800,600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	float lastFrameTicks = 0.0f;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}