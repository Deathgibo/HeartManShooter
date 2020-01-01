#pragma once
#include "PointLight.h"
#include "Model.h"
#include "ParticleSpawner_Fire.h"

class Torch
{
public:
	Torch();
	
	void Initialize(glm::vec3 pos, float con, float lin, float exp, glm::vec3 col, float inten, Model& mod, float x, float y, float z);
	void SetParticleSpawner(glm::vec3 pos, float life, float velocity, int spawn, int xa, int za, int rendersiz, int delayz);
	void Render(Shader* shader);
	void RenderScale(Shader* shader, float x, float y, float z);
	~Torch();

	PointLight pointlight;
	Model* model;
	glm::vec3 modeloffset;

	ParticleSpawner_Fire particlespawner;
private:

};

