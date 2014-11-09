#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(unsigned int particleCount) {
	//particles.resize(particleCount);
	maxLifetime = 2.0f;

	for (int i = 0; i < particleCount; i++) {
		Particle newParticle;
		newParticle.lifetime = ((float)rand() / (float)RAND_MAX) * maxLifetime;
		newParticle.position = position;
		particles.push_back(newParticle);
	}
}

ParticleEmitter::ParticleEmitter() {}
ParticleEmitter::~ParticleEmitter() {}

void ParticleEmitter::Update(float elapsed) {
	for (int i = 0; i < particles.size(); i++) {

		particles[i].position = particles[i].position + (particles[i].velocity * elapsed);

		particles[i].lifetime += elapsed;

	}
}

void ParticleEmitter::Render() {
	std::vector<float> particleVertices;
	for (int i = 0; i < particles.size(); i++) {
		particleVertices.push_back(particles[i].position.x);
		particleVertices.push_back(particles[i].position.y);
	}
	glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
}

void ParticleEmitter::trigger() {
	for (int i = 0; i < particles.size(); i++) {
			particles[i].lifetime = 0.0f;

			particles[i].velocity.x = velocity.x - (velocityDeviation.x * 0.5f) + (velocityDeviation.x * (((float)rand() / (float)RAND_MAX)));
			particles[i].velocity.y = velocity.y - (velocityDeviation.y * 0.5f) + (velocityDeviation.y * (((float)rand() / (float)RAND_MAX)));


			particles[i].position = position;

	}
}