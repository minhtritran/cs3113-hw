#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"
#include <math.h>
#include <string>

#define MAX_BULLETS 30

using namespace std;

class SpaceInvaders{
public:
	SpaceInvaders();
	~SpaceInvaders();
	void shootBullet();
	void Init();
	bool isColliding(const Entity& e1, const Bullet& e2);
	void Update(float elapsed);
	void UpdateMainMenu(float elapsed);
	void UpdateGameLevel(float elapsed);
	void UpdateGameOver(float elapsed);
	void UpdateGameStageCleared(float elapsed);
	void Render();
	void RenderMainMenu();
	void RenderGameLevel();
	void RenderGameOver();
	void RenderGameStageCleared();
	bool UpdateAndRender();
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);
	

private:
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	vector<Entity*> entities;

	int bulletIndex;
	Bullet bullets[MAX_BULLETS];

	GLuint spriteSheetTexture;
	GLuint fontSheetTexture;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER, STATE_GAME_STAGE_CLEARED };
	int state;

	float shootTimer;

	int difficulty;
	int score;
};