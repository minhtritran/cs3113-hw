#pragma once

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Particle.h"

class ParticleEmitter {
public:
	ParticleEmitter(unsigned int particleCount);
	
	ParticleEmitter();
	~ParticleEmitter();
	
	void Update(float elapsed);
	void Render();

	void trigger();

	Vector position;
	Vector velocity;
	Vector velocityDeviation;
	float maxLifetime;
	std::vector<Particle> particles;
};