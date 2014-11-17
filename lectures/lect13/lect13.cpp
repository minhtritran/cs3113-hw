app::
	ParticleEmitter demoParticles(30);

	demoParticles.startColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	demoParticles.endColor = Color(1.0f, 1.0f, 1.0f, 0.0f);

	demoParticles.maxLifetime = 1.0f;

	demoParticles.colorDeviation = Color(0.0f, 0.0f, 0.0f, 1.0f);

	demoParticles.position = Vector(0.0f, 2.0f, 0.0f);
	demoParticles.velocity = Vector(0.0f, 2.0f, 0.0f);
	demoParticles.velocityDeviation = Vector(0.5f, 5.0f, 0.0f);

ParticleEmitter::ParticleEmitter(unsigned int particleCount) {
	particles.resize(particleCount);

	maxLifetime = 2.0f;
	for particle.size() {
		particles[i].lifetime = ((float)rand() / (float)RAND_MAX) * maxLifetime;
	}

	gravity = Vector(0.0f, -5.0f, 0.0f);
}

void ParticleEmitter::Update(float elapsed) {
	for particles.size() {
		particles[i].velocity += (gravity * elapsed);
		particle[i].position += (particles[i].velocity * elapsed);

		particles[i].lifetime += elapsed;

		if particles[i].lifetime > maxLifetime {
			particles[i].lifetime = 0.0f;
			//with deviation
			particles[i].velocity.x = velocity.x -(velocityDeviation.x * 0.5f) + (velocityDeviation.x * (((float)rand() / (float)RAND_MAX))
			//do y too

			//without deviation
			//particles[i].velocity = Vector();

			particles[i].position = position;

			//do color
		}
	}
}