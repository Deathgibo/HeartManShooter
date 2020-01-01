#pragma once
#include "Particles.h"
#include "Square.h"
#include "ParticleMesh.h"
#include "Shader.h"
#include "Utils.h"
#include "Camera.h"
#include "Texture.h"

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <algorithm>

using namespace std;

class ParticleSpawner_Fire
{
public:
	ParticleSpawner_Fire(float width, float height, float capac);
	ParticleSpawner_Fire();

	void Initialize(float width, float height, float capac);
	void Update();
	void Render(Shader* shader);

	~ParticleSpawner_Fire();

	vector<Particles> particles;
	glm::vec3 particlecenter;
	float particlelife;
	int xarea;
	int zarea;
	float particlevelocity;
	int spawnspeed;
	int delay;
	int delaycounter = 0;

	int capacity;
	int lastusedparticle;

	//Square model;
	ParticleMesh model;

	GLfloat* positiondata;
	GLfloat* colordata;
	GLfloat* angledata;
private:

	int FirstUnusedParticle();
	void Respawn(int index);
};

