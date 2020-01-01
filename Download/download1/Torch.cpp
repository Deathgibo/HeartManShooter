#include "Torch.h"

Torch::Torch()
{

}

void Torch::Initialize(glm::vec3 pos, float con, float lin, float exp, glm::vec3 col, float inten, Model& mod, float x, float y, float z)
{
	pointlight.Initialize(pos, con, lin, exp, col, inten);
	model = &mod;
	modeloffset = glm::vec3(x, y, z);
}

void Torch::SetParticleSpawner(glm::vec3 pos, float life, float velocity, int spawn, int xa, int za, int rendersize, int delayz)
{
	particlespawner.Initialize(rendersize, rendersize, 100);
	particlespawner.particlelife = life;
	particlespawner.particlevelocity = velocity;
	particlespawner.spawnspeed = spawn;
	particlespawner.xarea = xa;
	particlespawner.zarea = za;
	particlespawner.particlecenter = pos;
	particlespawner.delay = delayz;
	particlespawner.delaycounter = 0;
}

void Torch::Render(Shader* shader)
{
	extern glm::mat4 mMat;
	extern glm::mat4 vMat;
	extern glm::mat4 mvMat;

	glUseProgram(shader->program);
	mMat = glm::mat4(1.0f);
	mMat = glm::translate(mMat, pointlight.position + modeloffset);
	//mMat = glm::scale(mMat, glm::vec3(15.0f, 15.0f, 15.0f));
	mvMat = vMat * mMat;
	glUniformMatrix4fv(shader->mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(shader->mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	model->RenderModel();


}

void Torch::RenderScale(Shader* shader, float x, float y, float z)
{
	extern glm::mat4 mMat;
	extern glm::mat4 vMat;
	extern glm::mat4 mvMat;

	glUseProgram(shader->program);
	mMat = glm::mat4(1.0f);
	mMat = glm::translate(mMat, pointlight.position + modeloffset);
	mMat = glm::scale(mMat, glm::vec3(x, y, z));
	mvMat = vMat * mMat;
	glUniformMatrix4fv(shader->mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(shader->mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	model->RenderModel();
}

Torch::~Torch()
{

}
