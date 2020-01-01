#include "ParticleSpawn_Fog.h"

ParticleSpawn_Fog::ParticleSpawn_Fog()
{
	capacity = 0;
	lastusedparticle = 0;

	positiondata = nullptr;
	colordata = nullptr;
	angledata = nullptr;
}
ParticleSpawn_Fog::ParticleSpawn_Fog(float width, float height, float capac)
{
	model.LoadParticleIndex(capac, width, height);
	capacity = capac;
	lastusedparticle = 0;

	positiondata = new GLfloat[capacity * 3];
	colordata = new GLfloat[capacity * 4];
	angledata = new GLfloat[capacity];

	for (int i = 0; i < capac; i++)
	{
		particles.push_back(Particles());
	}
	for (int i = 0; i < capac; i++)
	{
		particles[i].cameradistance = -1.0f;
	}
}

void ParticleSpawn_Fog::Initialize(float width, float height, float capac)
{
	model.LoadParticleIndex(capac, width, height);
	capacity = capac;
	lastusedparticle = 0;

	positiondata = new GLfloat[capacity * 3];
	colordata = new GLfloat[capacity * 4];
	angledata = new GLfloat[capacity];

	for (int i = 0; i < capac; i++)
	{
		particles.push_back(Particles());
	}
	for (int i = 0; i < capac; i++)
	{
		particles[i].cameradistance = -1.0f;
	}
}

void ParticleSpawn_Fog::Update()
{
	extern Camera camera;

	//Add new particles
	for (int i = 0; i < 10; i++)
	{
		int unusedparticle = FirstUnusedParticle();
		Respawn(unusedparticle);
	}

	//Update all particles
	int particlescount = 0;
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i].life > 0)
		{
			particles[i].life--;

			if (particles[i].life > 0)
			{

				particles[i].cameradistance = Utils::Distance3vec(particles[i].position, camera.position);

				particles[i].position += particles[i].velocity;
				//particles[i].color.z = (particles[i].life / 70.0f) + .2f;
				//particles[i].color.y = (particles[i].life / 70.0f) + .6f;

				/*update buffers*/
				positiondata[3 * particlescount + 0] = particles[i].position.x;
				positiondata[3 * particlescount + 1] = particles[i].position.y;
				positiondata[3 * particlescount + 2] = particles[i].position.z;

				colordata[4 * particlescount + 0] = particles[i].color.x;
				colordata[4 * particlescount + 1] = particles[i].color.y;
				colordata[4 * particlescount + 2] = particles[i].color.z;
				colordata[4 * particlescount + 3] = particles[i].color.w;

				angledata[particlescount] = particles[i].angle;
			}
			else
			{
				particles[i].cameradistance = -1.0f;
			}

			particlescount++;
		}
	}
	if(!particles.empty())
		sort(&particles[0], &particles[capacity-1]);
}

void ParticleSpawn_Fog::Render(Shader* shader)
{
	extern glm::mat4 mMat;
	extern glm::mat4 vMat;
	extern glm::mat4 mvMat;
	extern glm::mat4 pMat;
	extern Camera camera;
	extern Shader shadermain;
	extern Texture fogtexture;

	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = rotation * Utils::GetRodriguezMatrix(camera.rightdirection, glm::radians(-camera.pitch + 90.0f));
	rotation = rotation * Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-camera.yaw - 90.0f + 180.0f));
	glEnable(GL_BLEND);
	//mvMat = glm::mat4(1.0f);
	glUseProgram(shader->program);
	glUniformMatrix4fv(shader->projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(shader->mvLoc, 1, GL_FALSE, glm::value_ptr(rotation));
	glUniformMatrix4fv(shader->vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniform1i(shader->textureused, 1);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	fogtexture.UseTexture();
	model.UpdateBind(capacity, positiondata, colordata, angledata);
	model.RenderMesh(capacity);

	glUniform1i(shader->textureused, 0);
	/*for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i].life > 0)
		{
			mMat = glm::mat4(1.0f);
			mMat = glm::translate(mMat, particles[i].position);
			mMat = mMat * particles[i].rotatematrix;
			mvMat = vMat * mMat;
			glUniformMatrix4fv(shader->mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
			//glUniform4fv(shader->color, 1, glm::value_ptr(particles[i].color));
			model.RenderMesh(capacity);
		}
	}*/
	glDisable(GL_BLEND);
}

int ParticleSpawn_Fog::FirstUnusedParticle()
{
	for (int i = lastusedparticle; i < particles.size(); i++)
	{
		if (particles[i].life <= 0)
		{
			lastusedparticle = i;
			return i;
		}
	}
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i].life <= 0)
		{
			lastusedparticle = i;
			return i;
		}
	}

	cout << "all particles used" << endl;
	lastusedparticle = 0;
	return 0;
}

void ParticleSpawn_Fog::Respawn(int index)
{
	int randomx, randomz, randomy, randomlife;
	randomx = rand() % 250 + 600;
	randomz = rand() % 1000 - 500;
	randomy = rand() % 20 + 20;
	randomlife = rand() % 40 + 150;
	particles[index].position = glm::vec3((randomx / 100.0f) * 10, 5, (randomz / 100.0f) * 10);
	particles[index].velocity = glm::vec3(0, .2f, 0);
	particles[index].color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
	particles[index].rotatematrix = Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), Utils::ToRadians(rand() % 360));
	particles[index].angle = Utils::ToRadians(rand() % 360);
	particles[index].life = randomlife;
}

ParticleSpawn_Fog::~ParticleSpawn_Fog()
{
	if (positiondata != nullptr)
		delete[] positiondata;
	if (colordata != nullptr)
		delete[] colordata;
	if (angledata != nullptr)
		delete[] angledata;
}
