#pragma once

#include "SheetSprite.h"
class Bullet {
public:
	Bullet();
	Bullet(SheetSprite sprite, float scale, float x, float y, float rotation, float speed);
	void Update(float elapsed);
	void Render();

	SheetSprite sprite;

	float scale;
	float x;
	float y;
	float rotation;
	float speed;
	float visible;
};