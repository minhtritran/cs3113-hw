#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

class Entity {
public:
	Entity(float x, float y, float rotation, float width, float height, float speed, float red, float green, float blue)
		: x(x), y(y), rotation(rotation), width(width), height(height), speed(speed), red(red), green(green), blue(blue) {}
	void Draw() {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glTranslatef(x, y, 0.0);
		glRotatef(rotation, 0.0, 0.0, 1.0);

		GLfloat quad[] = { -width * 0.5f, height * 0.5f, -width * 0.5f, -height * 0.5f, width * 0.5f, -height * 0.5f, width * 0.5f, height * 0.5f };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat quadColors[] = { red, green, blue, red, green, blue, red, green, blue, red, green, blue };
		glColorPointer(3, GL_FLOAT, 0, quadColors);
		glEnableClientState(GL_COLOR_ARRAY);

		GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_COLOR_ARRAY);
	}
	void setColor(float red, float green, float blue) {
		this->red = red;
		this->green = green;
		this->blue = blue;
	}
	float x;
	float y;
	float rotation;

	GLuint textureID;

	float width;
	float height;

	float speed;
	float direction_x;
	float direction_y;

	float red;
	float green;
	float blue;
};