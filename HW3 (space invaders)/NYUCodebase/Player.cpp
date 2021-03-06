#include "Player.h"

Player::Player(SheetSprite sprite, float scale, float x, float y, float rotation, float speed, float direction_x)
	: Entity(sprite, scale, x, y, rotation, speed, direction_x)	 {}

void Player::Update(float elapsed) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_A]) {
		direction_x = -speed;
	}
	else if (keys[SDL_SCANCODE_D]) {
		direction_x = speed;
	}

	if ((!(x > 1.2) || direction_x < 0) && (!(x < -1.2) || direction_x > 0))
		x += direction_x * speed * elapsed;

	direction_x = 0;
}