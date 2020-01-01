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

class ParticleSpawn_Fog
{
public:
	ParticleSpawn_Fog(float width, float height, float capac);
	ParticleSpawn_Fog();

	void Initialize(float width, float height, float capac);
	void Update();
	void Render(Shader* shader);

	~ParticleSpawn_Fog();

	vector<Particles> particles;
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

