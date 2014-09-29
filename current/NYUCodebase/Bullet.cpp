#include "Bullet.h"

Bullet::Bullet() {

}

void Bullet::Update(float elapsed) {
	x += cos(angle * PI / 180.0f) * elapsed * speed;
	y += sin(angle * PI / 180.0f) * elapsed * speed;
	timeAlive += elapsed;
}

void Bullet::Draw() {
	if (!visible) {
		return;
	}

	GLfloat quad[] = { -size, size, -size, -size, size, -size, size, size };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
}