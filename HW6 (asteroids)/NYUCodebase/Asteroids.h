#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"
#include "ParticleEmitter.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define MAX_BULLETS 30
#define MAX_BRICKS 200
#define MAX_ENEMIES 8
#define PI 3.141592653589793238

using namespace std;

class Asteroids{
public:
	Asteroids();
	~Asteroids();

	void Init();
	void Update(float elapsed);
	void FixedUpdate(float fixedElapsed);
	void Render();
	bool UpdateAndRender();

	bool checkCollision(Entity* entity1, Entity* entity2);
	void shootBullet();

private:
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	GLuint spriteSheetTexture;
	SheetSprite bulletSprite;

	float gravity_x;
	float gravity_y;

	int bulletIndex;
	Bullet bullets[MAX_BULLETS];

	int enemyIndex;
	Entity enemies[MAX_ENEMIES];

	unsigned int mapWidth;
	unsigned int mapHeight;

	Player* player;
	vector<Entity*> entities;

	float shootTimer;
	float enemySpawnTimer;

	Mix_Chunk* gunshot;
	Mix_Music* music;

	float playerStartAnimationTime;

	vector<ParticleEmitter> particleEmitters;
};
float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);
float lerp(float v0, float v1, float t);
float easeIn(float from, float to, float time);
float easeOutElastic(float from, float to, float time);
float genRandomNumber(float low, float high);