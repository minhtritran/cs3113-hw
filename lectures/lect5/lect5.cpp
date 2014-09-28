
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

void DrawBg();

//This can be a sprite class, so that we only need to pass in the index
void DrawSpritesheetSprite(int spriteTexture, int index, int spriteCountX, int spriteCountY) {
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float spriteWidth = ;	//missing stuff
	float spriteHeight = ;	//missing stuff

	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat uvs[] = {u, v, u, v+spriteHeight, u+spriteWidth, v+spriteHeight, u+spriteWidth, v};
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, 4);
}

void DrawText(unsigned int spriteTexture, string text, float size, float spacing, float r, float g, float b, float a) {
	
	vector<float> vertexData;
	vector<float> uvData;
	vector<float> colorData;

	float spriteWidth = 1.0f / 16.0f;
	float spriteHeight = 1.0f / 16.0f;

	for (int i = 0; i < text.size(); i++) {
		float u = (float)((int)text[i] % 16) / 16.0f;
		float v = (float)((int)text[i] / 16) / 16.0f;

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size,
												((size+spacing) * i) + (-0.5f * size), -0.5f * size,
												((size + spacing) * i) + (0.5f * size), -0.5f * size,
												((size + spacing) * i) + (0.5f * size), 0.5f * size,
											});
		
		colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
		
		uvData.insert(uvData.end(), { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v });
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, uvData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);	//This is wrong
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

	int spriteTexture = LoadTexture("characters_3.png");
	int fontTexture = LoadTexture("font1.png");

	//This can be an animation class
	const int runAnimation[] = {9, 10, 11, 12, 13};
	const int numFrames = 5;
	float animationElapsed = 0.0f;
	float framesPerSecond = 30.0f;
	unsigned int currentIndex = 0;

	float lastFrameTicks = 0.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		animationElapsed += elapsed;
		
		if (animationElapsed > 1.0 / framesPerSecond) {
			currentIndex++;
			animationElapsed = 0.0f;

			if (currentIndex > numFrames - 1) {
				currentIndex = 0;
			}
		}

		DrawSpritesheetSprite(spriteTexture, runAnimation[currentIndex], 8, 4);

		glTranslatef(-0.5, 0.0, 0.0);

		DrawText(fontTexture, "Hello, World!", 0.1, -0.02, 1.0, 0.0, 0.0, 1.0);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}