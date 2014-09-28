#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float width, float height);

	void Draw(float scale, float x, float y, float rotation);

	GLuint textureID;
	float u;
	float v;
	float width;
	float height;
};

GLuint LoadTexture(const char *image_path);