#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>
#include "SheetSprite.h"
#include "Matrix.h"

class Entity {
public:
	Entity();
	~Entity();

	virtual void Update(float elapsed);
	virtual void Render();

	void FixedUpdate();

	void buildMatrix();

	SheetSprite sprite;

	Matrix matrix;

	float x;
	float y;
	float scale_x;
	float scale_y;
	float rotation;
	
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float friction_x;
	float friction_y;
	float mass;
	bool isStatic;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
	bool isJumping;
};