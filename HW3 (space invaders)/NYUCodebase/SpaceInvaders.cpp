#include "SpaceInvaders.h"

SpaceInvaders::SpaceInvaders() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	
	done = false;
	lastFrameTicks = 0.0f;
	state = STATE_MAIN_MENU;
	difficulty = 0;
	score = 0;
	Init();
}

void SpaceInvaders::shootBullet() {
	SheetSprite bulletSprite = SheetSprite(spriteSheetTexture, 856.0f / 1024.0f, 421.0f / 1024.0f, 9.0f / 1024.0f, 54.0f / 1024.0f);
	bullets[bulletIndex].sprite = bulletSprite;
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = entities[0]->getX();
	bullets[bulletIndex].y = -0.8f;
	bullets[bulletIndex].scale = 1.2f - (sqrt((float)difficulty*2)/6);
	bullets[bulletIndex].rotation = 0.0f;
	bullets[bulletIndex].speed = 2.0f;
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1) {
		bulletIndex = 0;
	}
	shootTimer = 0;
}

void SpaceInvaders::Init() {
	bulletIndex = 0;
	shootTimer = 0.1f;

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].visible = FALSE;
	}
	for (size_t i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	entities.clear();
	
	spriteSheetTexture = LoadTexture("sheet.png");
	fontSheetTexture = LoadTexture("pixel_font.png");

	SheetSprite playerSprite = SheetSprite(spriteSheetTexture, 425.0f / 1024.0f, 468.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f);
	Player* player = new Player(playerSprite, 0.9f, 0.0f, -0.85f, 180.0f, 1.0f);
	entities.push_back(player);

	SheetSprite enemySprite = SheetSprite(spriteSheetTexture, 423.0f / 1024.0f, 728.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f);
	
	for (float i = -1.0f; i < 1.0f; i = i + 0.2f) {
		for (float j = 0.85f; j > 0.0f; j = j - 0.2f) {
			Entity* enemy = new Entity(enemySprite, 0.7f, i, j, 0.0f, 1.0f * (1 + (float)difficulty / 2), 0.2f, -0.03f);
			entities.push_back(enemy);
		}
	}

}

SpaceInvaders::~SpaceInvaders() {
	SDL_Quit();
}

bool SpaceInvaders::isColliding(const Entity& e1, const Bullet& e2) {
	float e1_y_max = e1.y + e1.sprite.height / 2;
	float e1_y_min = e1.y - e1.sprite.height / 2;
	float e1_x_max = e1.x + e1.sprite.width / 2;
	float e1_x_min = e1.x - e1.sprite.width / 2;

	float e2_y_max = e2.y + e2.sprite.height / 2;
	float e2_y_min = e2.y - e2.sprite.height / 2;
	float e2_x_max = e2.x + e2.sprite.width / 2;
	float e2_x_min = e2.x - e2.sprite.width / 2;

	if (e1_y_min > e2_y_max)
		return false;
	if (e1_y_max < e2_y_min)
		return false;
	if (e1_x_min > e2_x_max)
		return false;
	if (e1_x_max < e2_x_min)
		return false;

	return true;
}


void SpaceInvaders::Update(float elapsed) {
	switch (state) {
	case STATE_MAIN_MENU:
		UpdateMainMenu(elapsed);
		break;
	case STATE_GAME_LEVEL:
		UpdateGameLevel(elapsed);
		break;
	case STATE_GAME_OVER:
		UpdateGameOver(elapsed);
		break;
	case STATE_GAME_STAGE_CLEARED:
		UpdateGameStageCleared(elapsed);
		break;
	}
}

void SpaceInvaders::UpdateMainMenu(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				state = STATE_GAME_LEVEL;
			}
		}
	}

}
void SpaceInvaders::UpdateGameLevel(float elapsed) {
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
	
	for (size_t i = 1; i < entities.size(); i++) {
		//game over if enemy ships pass a certain point
		if (entities[i]->getY() < -0.7f) {
			state = STATE_GAME_OVER;
			score = 0;
			Init();
		}
		//make enemy ships go left and right uniformly when hitting the edges
		if (entities[i]->getX() > 1.2 || entities[i]->getX() < -1.2) {
			for (size_t k = 1; k < entities.size(); k++) {
				entities[k]->direction_x = -entities[k]->direction_x;
			}
			break;
		}
		//remove enemy ship and bullet if it gets hit
		for (size_t j = 0; j < MAX_BULLETS; j++) {
			if (bullets[j].visible && isColliding(*entities[i], bullets[j])) {
				bullets[j].visible = false;
				delete entities[i];
				entities.erase(entities.begin() + i);
				score++;
				break;
			}
		}
	}

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Update(elapsed);
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Update(elapsed);
	}
	//stage cleared
	if (entities.size() <= 1) {
		state = STATE_GAME_STAGE_CLEARED;
		difficulty++;
		Init();
	}

	shootTimer += elapsed;
}
void SpaceInvaders::UpdateGameOver(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				state = STATE_MAIN_MENU;
			}
		}
	}
}
void SpaceInvaders::UpdateGameStageCleared(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				state = STATE_GAME_LEVEL;
			}
		}
	}
}

void SpaceInvaders::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state) {
		case STATE_MAIN_MENU:
			RenderMainMenu();
			break;
		case STATE_GAME_LEVEL:
			RenderGameLevel();
			break;
		case STATE_GAME_OVER:
			RenderGameOver();
			break;
		case STATE_GAME_STAGE_CLEARED:
			RenderGameStageCleared();
			break;
	}
	

	SDL_GL_SwapWindow(displayWindow);
}

void SpaceInvaders::RenderMainMenu() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.66f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "Space Invaders", 0.1, 0.0, 1.0, 1.0, 0.4, 1.0);
	glLoadIdentity();
	glTranslatef(-0.5f, -0.2f, 0.0f);
	DrawText(fontSheetTexture, "Press SPACE to start", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
}
void SpaceInvaders::RenderGameLevel() {
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Render();
	}
	glLoadIdentity();
	glTranslatef(-1.3f, 0.96f, 0.0f);
	DrawText(fontSheetTexture, "Score: " + to_string(score), 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
}
void SpaceInvaders::RenderGameOver() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.4f, 0.2f, 0.0f);
	DrawText(fontSheetTexture, "GAME OVER", 0.1, 0.0, 1.0, 1.0, 0.4, 1.0);
	glLoadIdentity();
	glTranslatef(-0.2f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "You lost!", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.85f, -0.5f, 0.0f);
	DrawText(fontSheetTexture, "Press ESCAPE to go back to main menu", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
}
void SpaceInvaders::RenderGameStageCleared() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.6f, 0.2f, 0.0f);
	DrawText(fontSheetTexture, "STAGE CLEARED", 0.1, 0.0, 1.0, 1.0, 0.4, 1.0);
	glLoadIdentity();
	glTranslatef(-0.45f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "Difficulty increased", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.9f, -0.5f, 0.0f);
	DrawText(fontSheetTexture, "Press ESCAPE to continue to next level", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
}

bool SpaceInvaders::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	Update(elapsed);
	Render();
	return done;
}

void SpaceInvaders::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f* size), 0.5f* size, ((size + spacing) * i) + (-0.5f* size), -0.5f* size, ((size + spacing) * i) + (0.5f* size), -0.5f* size, ((size + spacing) * i) + (0.5f* size), 0.5f * size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}
	
	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
}