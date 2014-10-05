#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define MAX_BULLETS 30
#define MAX_BRICKS 200
#define MAX_ENEMIES 8

using namespace std;

class PlatKillas{
public:
	PlatKillas();
	~PlatKillas();

	void Init();
	void Update(float elapsed);
	void FixedUpdate();
	void Render();
	bool UpdateAndRender();

	void buildLevel();
	void shootBullet();
	

private:
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	GLuint characterSpriteSheetTexture;
	GLuint brickSpriteSheetTexture;

	float gravity_x;
	float gravity_y;

	int bulletIndex;
	Bullet bullets[MAX_BULLETS];

	int brickIndex;
	Entity bricks[MAX_BRICKS];

	int enemyIndex;
	Entity enemies[MAX_ENEMIES];

	Player* player;
	vector<Entity*> entities;

	float shootTimer;
	float enemySpawnTimer;

};

float lerp(float v0, float v1, float t);