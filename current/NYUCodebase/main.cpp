
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

int main() {
	return 0;
}

unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];

unsigned char levelData[0] = {0,20,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4};

void ClassDemoApp::buildLevel() {
	memcpy(levelData, level1Data, LEVEL_HEIGHT*LEVEL_WIDTH);
}

//level1Data.h that contains the level1data
//have an unsigned char levelData[16][22] in CLassDemoApp.h


//put this function into Render
void ClassDemoApp::RenderLevel() {
	glBindTexture(GL_TEXTURE_2D, spriteSheetTexture);
	glenable texture
	glenable blend
	glblendfunc

	vector<float> vertexData;
	vector<float> textCoordData;

	int numVertices = 0;

	for int y
		for int x
			if levelData[y][x] > 0
				float u =
				float v =

				float spriteWidth = 1.0f / 16.0f * 0.999f;
				float spriteHeight = 1.0f / 8.0f * 0.999f;

				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					.....

				})

				txtCoordData.insert(....)

				numVertices += 4;


	glmatrixmode modelview
	glloadidentity
	gl translatef(...)

	gldisableclientstate 
}

bool ClassDemoApp::isSolid(unsigned char tile) {
	switch (tile) {
	case 1:
	case 2:
	case 3:
	case 20:
		return true;
		break;
	default:
		return false;
		break;
	}
}

float ClassDemoApp::checkPointForGridCollisionX(float x, float y) {
	int gridX, gridY;
	worldToTilesCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 21 || gridY < 0 || gridY > 15) {
		return 0.0f;
	}

	if (isSolid(levelData[gridX][gridY])) {
		float yCoordinate = (gridY * TILE_SIZE) - (TILE_SIZE*8.0);
		return - y - yCoordinate;
	}
	return 0.0f;
}

void ClassDemoApp::doLevelCollisionY(Entity *entity) {
	//check bottom

	float adjust = checkPointForGridCollisionY(entity->x, entity->y - entity->height*0.5);
	if (adjust != 0.0f) {
		entity->y += adjust;
		entity->velocity_y = 0.0f;
		entity->collidedBotom = true;
	}
	
	//check top

	//blah blah blah
}

void ClassDemoApp::FixedUpdate() {
	for entities.size
		entities[i]->fixedupdate
		entities[i]->collidedTop = false;
		bottom, right, left = false;
	
		doLevelCollisionY
		doLevelCollisionX
}

//for scrolling
void CLassDemoApp::Render() {
	glClearColor;
	glClear;

	float translateX = -player.legsEntity.x;
	float translateY = -player.legsENtity.y;

	if (translateY > 0.0)
		translateY = 0.0;
	if (translateX > 0.0)
		translateX = 0.0;
	if (traslateX < -TILE_SIZE * (LEVEL_WIDTH / 2)) {
		translateX = -TILE_SIZE * (LEVEL_WIDTH / 2);
	}


}

void Entity::Render() {
	if (!visible)
		return;
	glPushMatrix;
	glTransalte;
	glrotate;
	sprite.Draw();
	glpopmatrx();
}


//bullet
void Bullet::Update(float elapsed) {
	x += cos(angle * PI / 180.0f) * elapsed * speed;
	y += sin(angle * PI / 180.0f) * elapsed * speed;
	timeAlive += elapsed;
}
void Bullet::Draw() {
	if visible
		return;
	glPushMatrix();
	gltranslatef(x, y, 0.0f);
	glrotate(angle, 0, 0, 1.0f);
	sprite.draw();
	glPopMatrix();
}