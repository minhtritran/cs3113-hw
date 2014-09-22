
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>

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

	glViewport(0,0,800,600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	GLuint emojiTexture = LoadTexture("emoji.png");

	float lastFrameTicks = 0.0f;
	float faceRotation = 0.0;
	
	float posX = 0.0;
	float posY = 0.0;

	float xMove = 0.0;
	float yMove = 0.0;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			/*else if (event.type == SDL_MOUSEMOTION) {
				float unitX = (((float)event.motion.x / 800.0f) * 2.666f) - 1.333f;
				float unitY = (((float)(600 - event.motion.y) / 600.0f) * 2.0f) - 1.0f;

				posX = unitX;
				posY = unitY;
			}*/
			else if (event.type == SDL_JOYAXISMOTION) {

				if (event.jaxis.axis == 0) {
					xMove = ((float)event.jaxis.value / 32767.0f) * 3.0;
				}
				else if (event.jaxis.axis == 1) {
					yMove = ((float)event.jaxis.value / 32767.0f) * 3.0;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {

				if (event.button.button == 3) {
					float unitX = (((float)event.motion.x / 800.0f) * 2.666f) - 1.333f;
					float unitY = (((float)(600 - event.motion.y) / 600.0f) * 2.0f) - 1.0f;

					posX = unitX;
					posY = unitY;
				}
			}
		}
		
		float ticks = (float)SDL_GetTicks()/1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		if (xMove > 0.3) {
			posX += xMove * elapsed;
		}

		if (yMove > 0.3) {
			posY -= yMove * elapsed;
		}
		
		faceRotation += 45.0f * elapsed;
		DrawSprite(emojiTexture, posX, posY, faceRotation);


		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}