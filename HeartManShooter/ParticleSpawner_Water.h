#pragma once
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

class ParticleSpawner_Water
{
public:
	ParticleSpawner_Water(float width, float height, float capac);
	ParticleSpawner_Water();

	void Initialize(float width, float height, float capac);
	void Update();
	void Render(Shader* shader);

	~ParticleSpawner_Water();

	vector<Particles> particles;
	glm::vec3 spawncenter;
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

