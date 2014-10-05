
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;
	
	SDL_Event event;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}

void ClassDemoApp::FixedUpdate() {
	for (int i = 0; i < entities.size(); i++) {
		entities[i].FixedUpdate();
		if (!entities[i]->isStatic) {
			entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}
		
	}

	for (int i = 0; i < entities.size(); i++) {

		//y axis

		entities[i]->y = += entities->velocity_y * FIXED_TIMESTEP;

		for (int j = 0; j < entities.size(); j++) {
			if (entities[j] != entities[i]) {
				if (entities[i]->collidesWith(*entities[j])) {
					float yPenetration = fabs(entities[j]->y - entities[i]->y) - entities[i]->height / 2.0f - entities[j]->height / 2.0f;

					if (entities[j]->y > entities[i]->y) {
						entities[j]->y -= yPenetration + 0.001f;
					}
					else {
						entities[j]->y += yPenetration + 0.001f;
					}
				}
			}
		}

		//x axis

		entities[i]->x = += entities->velocity_x * FIXED_TIMESTEP;

		for (int j = 0; j < entities.size(); j++) {
			if (entities[j] != entities[i]) {
				if (entities[i]->collidesWith(*entities[j])) {
					float xPenetration = fabs(entities[j]->x - entities[i]->x) - entities[i]->width / 2.0f - entities[j]->width / 2.0f;

					if (entities[j]->x > entities[i]->x) {
						entities[j]->x -= xPenetration + 0.001f;
					}
					else {
						entities[j]->x += xPenetration + 0.001f;
					}

					
				}
			}
		}
		
		

	}
}

//#define other stuff
#define MAX_TIMESTEPS 6
#define RANDOM_NUMBER ((float)rand()/(float)RAND_MAX)
#define NUM_ENEMIES 8

ClassDemoApp::ClassDemoApp() {
	Init();
	buildLevel();

	vector<int> frames;
	frames.insert(frames.begin(), { 80, 81 });

	for (int i = 0; i < NUM_ENEMIES; i++) {
		enemies[i].sprite = SheetSprite(spriteSheetTexture, 80);
		enemies[i].sprite.setAnimated(true, 10.0f, frames);
		//set width, height, x, y, acceleration_x, mass, friction_x
		entities.push_back(&enemies[i]);
	}

	//make player

	gravity_x = 0.0f;
	gravity_y = -9.8f;

	bulletIndex = 0;
	done = false;
	lastFrameTicks = 0.0f;	//for fixedUpdate

}

bool ClassDempApp::UpdateAndRender() {
	//float ticks, elapsed, lastFrameTicks
	SDL_Event event;

	//while pollevent stuff
	else if (even.type == SDL_KEYDOWN) {
		if (!event.key.repeat) {
			switch (event.key.keysym.scanmode) {
			case SDL_SCANCPDE_Z:
				//jump
				break;
			}
		}
	}

	//const Uint8
	if (keys[SDL_SCANCODE_RIGHT]) {
		player.setWalkRight();
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		player.setWalkLefts();
	}
	else {
		player.setIdle();
	}
	if (keys[SDL_SCANCODE_X]) {
		if (shootTimer > 0.1f) {
			shootTimer = 0.0f;
			shootBullet();
		}
	}

	shootTimer += elapsed;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP)

}

void ClassDemoApp::FixedUpdate() {
	//for loop
		entities[i].FixedUpdate();

		//update collidedTop, bottom,...
		//do Y collisions
		for (int j = 0; j < entities.size(); j++) {
			entities[j]->y += entities[j]->velocity_y * FIXED_TIMESTEP;
			if (!entities[j]->isStatic && entities[j]->enableCollisions) {
				for //blah blah balh
			}
		}
		//do X collisions

		for (int i = 0; i < NUM_ENEMIES; i++) {
			if (enemies[i].collidedRight) {
				eenemies[i].acceleration_x = -3.0f;
				enemies[i].sprite.flipX = true;
			}

			if (enemies[i].collidedLeft) {
				eenemies[i].acceleration_x = 3.0f;
				enemies[i].sprite.flipX = false;
			}

			if (enemies[i].y < -3.0) {
				enemies[i].y = 3.0f;
				enemies[i].x = -0.3f + (RANDOM_NUMBER * 0.6f);
				enemies[i].velocity_y = 0.0f;
				enemies[i].velocity_x = 0.0f;
				enemies[i].enableCollision = true;

			}

		}

		//hit player
		if (enemies[i].collidesWith(&player.bodyEntity()))
			//player dies

		//hit bullet

		//loop static etities
		

}


void ENtity::FixedUpdate() {
	if (!isStatic) {
		//velocity_x = lerp(velocity_x, 0.0f, ....);
		//...
	}
}

void Sheetsprite::setAnimated(bool val, float fps, std::vector<int> frames) {
	animated = val;
	framsPerSecond = fps;
	this->frames = frames;
}

void Sheetsprite::Draw() {
	int frameIndex = index;
	if (animated) {

	}
}