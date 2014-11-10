#include "Asteroids.h"

Asteroids::Asteroids() {
	Init();
	srand(time(NULL));
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	gravity_x = 0.0f;
	gravity_y = 0.0f;

	bulletIndex = 0;
	shootTimer = 0.1f;

	spriteSheetTexture = LoadTexture("sheet.png");
	bulletSprite = SheetSprite(spriteSheetTexture, 425.0f / 1024.0f, 468.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f);

	SheetSprite playerSprite = SheetSprite(spriteSheetTexture, 425.0f / 1024.0f, 468.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f);
	player = new Player();
	player->sprite = playerSprite;
	player->scale_x = 1.0f;
	player->scale_y = 1.0f;
	player->x = 0.0f;
	player->y = 0.0f;
	player->friction_x = 3.0f;
	player->friction_y = 3.0f;
	entities.push_back(player);

	SheetSprite asteroidSprite = SheetSprite(spriteSheetTexture, 224.0f / 1024.0f, 664.0f / 1024.0f, 101.0f / 1024.0f, 84.0f / 1024.0f);
	for (int i = 0; i < 20; i++) {
		Entity* asteroid = new Entity();
		asteroid->sprite = asteroidSprite;
		float scale = genRandomNumber(0.7f, 1.5f);
		asteroid->scale_x = scale;
		asteroid->scale_y = scale;
		asteroid->x = genRandomNumber(-1.33f, 1.33f);
		asteroid->y = genRandomNumber(-1.0f, 1.0f);
		asteroid->rotation = genRandomNumber(-90.0f, 90.0f);
		asteroid->velocity_x = genRandomNumber(-0.15f, 0.15f);
		asteroid->velocity_y = genRandomNumber(-0.15f, 0.15f);
		entities.push_back(asteroid);
	}

	ParticleEmitter demoParticles(300);
	demoParticles.maxLifetime = 1.0f;
	demoParticles.position = Vector(0.0f, 0.5f, 0.0f);
	demoParticles.velocity = Vector(0.8f, 0.5f, 0.0f);
	demoParticles.velocityDeviation = Vector(0.5f, 1.5f, 0.0f);
	particleEmitters.push_back(demoParticles);
	


	gunshot = Mix_LoadWAV("gunshot.wav");
	music = Mix_LoadMUS("music.wav");
	if (Mix_PlayMusic(music, -1) < 0) {
		cout << "Error";
	}

}

Asteroids::~Asteroids() {


	SDL_Quit();
}

void Asteroids::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void Asteroids::Update(float elapsed) {
	playerStartAnimationTime += elapsed;
	if (playerStartAnimationTime < 1.0f) {
		float animationAValue = mapValue(playerStartAnimationTime, 0.4f, 2.0f, 0.0f, 1.0f);
		player->scale_x = easeOutElastic(0.0, 1.0, animationAValue);
		player->scale_y = easeOutElastic(0.0, 1.0, animationAValue);
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Update(elapsed);
	}

	for (size_t i = 0; i < particleEmitters.size(); i++) {
		particleEmitters[i].Update(elapsed);
	}

	shootTimer += elapsed;
	enemySpawnTimer += elapsed;
}

void Asteroids::FixedUpdate(float fixedElapsed) {

	player->scale_y = mapValue(fabs(player->velocity_y), 0.0, 5.0, 1.0, 1.6);
	player->scale_x = mapValue(fabs(player->velocity_y), 5.0, 0.0, 0.8, 1.0);

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->FixedUpdate();

		
		

		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}


		entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, FIXED_TIMESTEP * entities[i]->friction_x);
		entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, FIXED_TIMESTEP * entities[i]->friction_y);

		entities[i]->velocity_x += entities[i]->acceleration_x * FIXED_TIMESTEP;
		entities[i]->velocity_y += entities[i]->acceleration_y * FIXED_TIMESTEP;

		if (!entities[i]->isStatic) {
			for (size_t j = 0; j < entities.size(); j++) {
				if (checkCollision(entities[i], entities[j])) {
					Vector distance = Vector(entities[i]->x - entities[j]->x, entities[i]->y - entities[j]->y, 0.0f);
					float distLength = distance.length();
					distance.normalize();

					entities[i]->x += distance.x * 0.0001f / pow(distLength, 2);
					entities[i]->y += distance.y * 0.0001f / pow(distLength, 2);

					entities[j]->x -= distance.x * 0.0001f / pow(distLength, 2);
					entities[j]->y -= distance.y * 0.0001f / pow(distLength, 2);
				}
			}
			
		}

		entities[i]->y += entities[i]->velocity_y * -cos(entities[i]->rotation) * FIXED_TIMESTEP;
		entities[i]->x += entities[i]->velocity_x * sin(entities[i]->rotation) * FIXED_TIMESTEP;
		

	}

	for (int i = 1; i < entities.size(); i++) {
		//enemy gets hit
		for (int k = 0; k < MAX_BULLETS; k++) {
			if (bullets[k].visible && checkCollision(entities[i], &bullets[k]) ) {
				bullets[k].visible = false;

				ParticleEmitter demoParticles(300);
				demoParticles.maxLifetime = 1.0f;
				demoParticles.position = Vector(entities[i]->x, entities[i]->y, 0.0f);
				demoParticles.velocity = Vector(sin(player->rotation)*bullets[k].velocity_x/2, -cos(player->rotation)*bullets[k].velocity_y/2, 0.0f);
				demoParticles.velocityDeviation = Vector(0.5f, 1.5f, 0.0f);
				particleEmitters.push_back(demoParticles);
				particleEmitters.back().trigger();

				if (entities[i]->scale_x < 1.0f) {
					delete entities[i];
					entities.erase(entities.begin() + i);
					break;
				}
				else {
					entities[i]->scale_x = entities[i]->scale_x * 0.67;
					entities[i]->scale_y = entities[i]->scale_y * 0.67;
					entities[i]->rotation = genRandomNumber(-90.0f, 90.0f);
				}
					
			
			}
		}
	}
}

void Asteroids::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Render();
	}

	for (size_t i = 0; i < particleEmitters.size(); i++) {
		particleEmitters[i].Render();
	}

	SDL_GL_SwapWindow(displayWindow);
}

bool Asteroids::UpdateAndRender() {
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
		player->velocity_x = 1.0f;
		player->velocity_y = 1.0f;
	}
	else if (keys[SDL_SCANCODE_S]) {
		player->velocity_x = 0.0f;
		player->velocity_y = 0.0f;
	}
	if (keys[SDL_SCANCODE_D]) {
		player->rotation -= 5.0f * elapsed;
	}
	else if (keys[SDL_SCANCODE_A]) {
		player->rotation += 5.0f * elapsed;
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
		FixedUpdate(fixedElapsed);
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();
	return done;
}

bool Asteroids::checkCollision(Entity* entity1, Entity* entity2) {
	if (entity1 == entity2)
		return false;
	entity1->buildMatrix();
	entity2->buildMatrix();

	Matrix entity1Inverse = entity1->matrix.inverse();
	Matrix entity2Inverse = entity2->matrix.inverse();

	float minX, maxX, minY, maxY;

	Vector ent2TL = Vector(-entity2->sprite.width * entity2->scale_x, entity2->sprite.height * entity2->scale_y, 0.0);
	Vector ent2BL = Vector(-entity2->sprite.width * entity2->scale_x, -entity2->sprite.height * entity2->scale_y, 0.0);
	Vector ent2TR = Vector(entity2->sprite.width * entity2->scale_x, entity2->sprite.height * entity2->scale_y, 0.0);
	Vector ent2BR = Vector(entity2->sprite.width * entity2->scale_x, -entity2->sprite.height * entity2->scale_y, 0.0);

	//transform to world coordinates
	ent2TL = entity2->matrix * ent2TL;
	ent2BL = entity2->matrix * ent2BL;
	ent2TR = entity2->matrix * ent2TR;
	ent2BR = entity2->matrix * ent2BR;

	//transform to entity1's space
	ent2TL = entity1Inverse * ent2TL;
	ent2BL = entity1Inverse * ent2BL;
	ent2TR = entity1Inverse * ent2TR;
	ent2BR = entity1Inverse * ent2BR;
	
	//check x axis
	minX = min(min(min(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	maxX = max(max(max(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	if (!(minX <= entity1->sprite.width * entity1->scale_x && maxX >= -entity1->sprite.width * entity1->scale_x))
		return false;
	//check y axis
	minY = min(min(min(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	maxY = max(max(max(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	if (!(minY <= entity1->sprite.height * entity1->scale_y && maxY >= -entity1->sprite.height * entity1->scale_y))
		return false;


	//do the same for entity1
	Vector ent1TL = Vector(-entity1->sprite.width * entity1->scale_x, entity1->sprite.height * entity1->scale_y, 0.0);
	Vector ent1BL = Vector(-entity1->sprite.width * entity1->scale_x, -entity1->sprite.height * entity1->scale_y, 0.0);
	Vector ent1TR = Vector(entity1->sprite.width * entity1->scale_x, entity1->sprite.height * entity1->scale_y, 0.0);
	Vector ent1BR = Vector(entity1->sprite.width * entity1->scale_x, -entity1->sprite.height * entity1->scale_y, 0.0);

	//transform to world coordinates
	ent1TL = entity1->matrix * ent1TL;
	ent1BL = entity1->matrix * ent1BL;
	ent1TR = entity1->matrix * ent1TR;
	ent1BR = entity1->matrix * ent1BR;

	//transform to entity2's space
	ent1TL = entity2Inverse * ent1TL;
	ent1BL = entity2Inverse * ent1BL;
	ent1TR = entity2Inverse * ent1TR;
	ent1BR = entity2Inverse * ent1BR;

	//check x axis
	minX = min(min(min(ent1TL.x, ent1BL.x), ent1TR.x), ent1BR.x);
	maxX = max(max(max(ent1TL.x, ent1BL.x), ent1TR.x), ent1BR.x);
	if (!(minX <= entity2->sprite.width * entity2->scale_x && maxX >= -entity2->sprite.width * entity2->scale_x))
		return false;
	//check y axis
	minY = min(min(min(ent1TL.y, ent1BL.y), ent1TR.y), ent1BR.y);
	maxY = max(max(max(ent1TL.y, ent1BL.y), ent1TR.y), ent1BR.y);
	if (!(minY <= entity2->sprite.height * entity2->scale_y && maxY >= -entity2->sprite.height * entity2->scale_y))
		return false;

	return true;
}

void Asteroids::shootBullet() {
	Mix_PlayChannel(-1, gunshot, 0);
	bullets[bulletIndex].sprite = bulletSprite;
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = player->x;
	bullets[bulletIndex].y = player->y;
	bullets[bulletIndex].rotation = player->rotation;
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1) {
		bulletIndex = 0;
	}
	shootTimer = 0;
}

float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
	float retVal = dstMin + ((value - srcMin) / (srcMax - srcMin) * (dstMax - dstMin));
	if (retVal < dstMin) {
		retVal = dstMin;
	}
	if (retVal > dstMax) {
		retVal = dstMax;
	}
	return retVal;
}

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

float easeIn(float from, float to, float time) {
	float tVal = time*time*time*time*time;
	return(1.0f - tVal)*from + tVal*to;
}

float easeOutElastic(float from, float to, float time) {
	float p = 0.3f;
	float s = p / 4.0f;
	float diff = (to - from);
	return from + diff + (diff*pow(2.0f, -10.0f*time) * sin((time - s)*(2 * PI) / p));
}

float genRandomNumber(float low, float high) {
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
}