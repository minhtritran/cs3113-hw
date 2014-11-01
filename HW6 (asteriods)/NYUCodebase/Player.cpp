#include "Player.h"

Player::Player() {}

void Player::Update(float elapsed) {
	
}

void Player::Render() {
	buildMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	sprite.Draw(scale_x, scale_y);
	glPopMatrix();
}
