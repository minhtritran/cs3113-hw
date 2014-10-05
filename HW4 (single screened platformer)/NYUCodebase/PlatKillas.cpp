#include "PlatKillas.h"

PlatKillas::PlatKillas() {
	Init();
	

	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	gravity_x = 0.0f;
	gravity_y = -9.8f;

	bulletIndex = 0;
	shootTimer = 0.1f;

	buildLevel();

	characterSpriteSheetTexture = LoadTexture("characters_1.png");

	SheetSprite playerSprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 15);
	player = new Player();
	player->sprite = playerSprite;
	player->y = -0.5f;
	player->width = 0.2f;
	player->height = 0.2f;
	player->friction_x = 3.0f;
	entities.push_back(player);
	
}

PlatKillas::~PlatKillas() {
	SDL_Quit();
}

void PlatKillas::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void PlatKillas::Update(float elapsed) {
	if (enemySpawnTimer > 1.0f && enemyIndex < 8) {
		SheetSprite enemySprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 2);
		enemies[enemyIndex].sprite = enemySprite;
		enemies[enemyIndex].y = 0.85f;
		enemies[enemyIndex].x = 0.65f;
		enemies[enemyIndex].width = 0.2f;
		enemies[enemyIndex].height = 0.2f;
		enemies[enemyIndex].acceleration_x = -3.0f;
		entities.push_back(&enemies[enemyIndex]);
		enemyIndex++;
		enemySpawnTimer = 0.0f;
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Update(elapsed);
	}

	shootTimer += elapsed;
	enemySpawnTimer += elapsed;
}

void PlatKillas::FixedUpdate() {

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->FixedUpdate();

		if (entities[i]->collidedBottom) {
			entities[i]->isJumping = false;
			entities[i]->velocity_y = 0.0f;
		}
		if (entities[i]->collidedTop)
			entities[i]->velocity_y = 0.0f;
		if (entities[i]->collidedLeft)
			entities[i]->velocity_x = 0.0f;
		if (entities[i]->collidedRight)
			entities[i]->velocity_x = 0.0f;

		entities[i]->collidedBottom = false;
		entities[i]->collidedTop = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;

		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}

		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, FIXED_TIMESTEP * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, FIXED_TIMESTEP * entities[i]->friction_y);

		entities[i]->velocity_x += entities[i]->acceleration_x * FIXED_TIMESTEP;
		entities[i]->velocity_y += entities[i]->acceleration_y * FIXED_TIMESTEP;

		entities[i]->y += entities[i]->velocity_y * FIXED_TIMESTEP;
		//do Y collisions
		if (!entities[i]->isStatic) {
			for (size_t j = 0; j < entities.size(); j++) {
				if (entities[i]->collidesWith(entities[j]) && entities[i] != entities[j]) {
					float yPenetration = fabs(fabs(entities[j]->y - entities[i]->y) - entities[i]->height / 2.0f - entities[j]->height / 2.0f);
					if (entities[i]->y > entities[j]->y) {
						entities[i]->y += yPenetration + 0.001f;
						entities[i]->collidedBottom = true;
					}
					else if (entities[i]->y < entities[j]->y) {
						entities[i]->y -= yPenetration + 0.001f;
						entities[i]->collidedTop = true;
					}
				}
			}
		}
		
		entities[i]->x += entities[i]->velocity_x * FIXED_TIMESTEP;
		//do X collisions
		if (!entities[i]->isStatic) {
			for (size_t j = 0; j < entities.size(); j++) {
				if (entities[i]->collidesWith(entities[j]) && entities[i] != entities[j]) {
					float xPenetration = fabs(fabs(entities[j]->x - entities[i]->x) - entities[i]->width / 2.0f - entities[j]->width / 2.0f);
					if (entities[i]->x > entities[j]->x) {
						entities[i]->x += xPenetration + 0.001f;
						entities[i]->collidedLeft = true;
					}
					else if (entities[i]->x < entities[j]->x) {
						entities[i]->x -= xPenetration + 0.001f;
						entities[i]->collidedRight = true;
					}
				}
			}
		}
	}


	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].collidedRight) {
			enemies[i].acceleration_x = -3.0f;
		}

		if (enemies[i].collidedLeft) {
			enemies[i].acceleration_x = 3.0f;
		}
		//enemy gets hit
		for (int k = 0; k < MAX_BULLETS; k++) {
			if (enemies[i].collidesWith(&bullets[k])) {
				bullets[k].visible = false;
				enemies[i].y = 1.3f;
				enemies[i].x = 0.0f;
			}
		}
		//player loses
		if (enemies[i].x < -1.33 || enemies[i].x > 1.33) {
			player->height -= 0.01f; //I did this it looks funny and because I didn't want to make a game over state
		}
			
	}
}

void PlatKillas::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Render();
	}
	

	SDL_GL_SwapWindow(displayWindow);
}

bool PlatKillas::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (shootTimer > 0.1f)
					shootBullet();
			}
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W]) {
		if (!player->isJumping) {
			player->jump();
		}
		
	}
	if (keys[SDL_SCANCODE_D]) {
		SheetSprite playerSprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 27);
		player->sprite = playerSprite;
		player->setWalkRight();
	}
	else if (keys[SDL_SCANCODE_A]) {
		SheetSprite playerSprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 15);
		player->sprite = playerSprite;
		player->setWalkLeft();
	}
	else {
		player->setIdle();
	}
	if (keys[SDL_SCANCODE_X]) {
		if (shootTimer > 0.1f) {
			shootTimer = 0.0f;
			shootBullet();
		}
	}

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP* MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP* MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();
	return done;
}

void PlatKillas::buildLevel() {
	brickIndex = 0;
	brickSpriteSheetTexture = LoadTexture("sheet_4.png");
	SheetSprite brickSprite;
	for (float i = -1.0f; i <= 1.0f; i = i + 0.2f) {
		brickSprite = SheetSprite(brickSpriteSheetTexture, 14, 8, 7 + rand() % 3);
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].y = -0.9f;
		bricks[brickIndex].x = i;
		bricks[brickIndex].width = 0.2f;
		bricks[brickIndex].height = 0.2f;
		entities.push_back(&bricks[brickIndex]);
		brickIndex++;
	}

	brickSprite = SheetSprite(brickSpriteSheetTexture, 14, 8, 27);
	for (float i = -0.4f; i <= 1.0f; i = i + 0.2f) {
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].y = i;
		bricks[brickIndex].x = -1.23f;
		bricks[brickIndex].width = 0.2f;
		bricks[brickIndex].height = 0.2f;
		entities.push_back(&bricks[brickIndex]);
		brickIndex++;
	}
	for (float i = -0.4f; i <= 1.0f; i = i + 0.2f) {
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].y = i;
		bricks[brickIndex].x = 1.23f;
		bricks[brickIndex].width = 0.2f;
		bricks[brickIndex].height = 0.2f;
		entities.push_back(&bricks[brickIndex]);
		brickIndex++;
	}

	for (float i = -0.4f; i <= 0.4f; i = i + 0.2f) {
		brickSprite = SheetSprite(brickSpriteSheetTexture, 14, 8, 11);
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].y = 0.1f;
		bricks[brickIndex].x = i;
		bricks[brickIndex].width = 0.2f;
		bricks[brickIndex].height = 0.2f;
		entities.push_back(&bricks[brickIndex]);
		brickIndex++;
	}

	for (float i = -1.05f; i <= -0.8f; i = i + 0.2f) {
		brickSprite = SheetSprite(brickSpriteSheetTexture, 14, 8, 11);
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].y = -0.4f;
		bricks[brickIndex].x = i;
		bricks[brickIndex].width = 0.2f;
		bricks[brickIndex].height = 0.2f;
		entities.push_back(&bricks[brickIndex]);
		brickIndex++;
	}

	for (float i = 1.05f; i >= 0.8f; i = i - 0.2f) {
		brickSprite = SheetSprite(brickSpriteSheetTexture, 14, 8, 11);
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].y = -0.4f;
		bricks[brickIndex].x = i;
		bricks[brickIndex].width = 0.3f;
		bricks[brickIndex].height = 0.2f;
		entities.push_back(&bricks[brickIndex]);
		brickIndex++;
	}


	
}

void PlatKillas::shootBullet() {
	SheetSprite bulletSprite = SheetSprite(characterSpriteSheetTexture, 12, 8, 3);
	bullets[bulletIndex].sprite = bulletSprite;
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = player->x;
	bullets[bulletIndex].y = player->y;
	bullets[bulletIndex].rotation = 0.0f;
	if (player->face_left)
		bullets[bulletIndex].velocity_x = -3.5f;
	else
		bullets[bulletIndex].velocity_x = 3.5f;
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1) {
		bulletIndex = 0;
	}
	shootTimer = 0;
}

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}