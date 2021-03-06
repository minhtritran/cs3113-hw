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
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define MAX_BULLETS 30
#define MAX_BRICKS 200
#define MAX_ENEMIES 8
#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 8
#define TILE_SIZE 0.2f

using namespace std;

class SideScroller{
public:
	SideScroller();
	~SideScroller();

	void Init();
	void Update(float elapsed);
	void FixedUpdate();
	void Render();
	bool UpdateAndRender();

	void buildLevel();
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	bool readEntityData(ifstream& stream);
	void placeEntity(string& type, float placeX, float placeY);
	void RenderLevel();
	void shootBullet();
	bool isSolid(unsigned char tile);
	void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
	float SideScroller::checkPointForGridCollisionX(float x, float y);
	float SideScroller::checkPointForGridCollisionY(float x, float y);
	void SideScroller::doLevelCollisionX(Entity* entity);
	void SideScroller::doLevelCollisionY(Entity* entity);
	

private:
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	GLuint characterSpriteSheetTexture;
	GLuint brickSpriteSheetTexture;
	SheetSprite bulletSprite;

	float gravity_x;
	float gravity_y;

	int bulletIndex;
	Bullet bullets[MAX_BULLETS];

	int brickIndex;
	Entity bricks[MAX_BRICKS];

	int enemyIndex;
	Entity enemies[MAX_ENEMIES];

	unsigned int mapWidth;
	unsigned int mapHeight;

	unsigned char** levelData;

	Player* player;
	vector<Entity*> entities;

	float shootTimer;
	float enemySpawnTimer;

	Mix_Chunk* gunshot;
	Mix_Chunk* jump;
	Mix_Music* music;

};

float lerp(float v0, float v1, float t);