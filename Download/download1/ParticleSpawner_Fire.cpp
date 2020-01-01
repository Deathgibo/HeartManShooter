#include "ParticleSpawner_Fire.h"

ParticleSpawner_Fire::ParticleSpawner_Fire()
{
	capacity = 0;
	lastusedparticle = 0;

	positiondata = nullptr;
	colordata = nullptr;
	angledata = nullptr;
}
ParticleSpawner_Fire::ParticleSpawner_Fire(float width, float height, float capac)
{
	model.LoadParticleIndex(capac,width, height);
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
		particles[i].position = glm::vec3(0, -1000, 0);
		particles[i].color = glm::vec4(0, 0, 0, 0);
	}

	for (int i = 0; i < capac; i++)
	{
		positiondata[3 * i + 0] = particles[i].position.x;
		positiondata[3 * i + 1] = particles[i].position.y;
		positiondata[3 * i + 2] = particles[i].position.z;

		colordata[4 * i + 0] = particles[i].color.x;
		colordata[4 * i + 1] = particles[i].color.y;
		colordata[4 * i + 2] = particles[i].color.z;
		colordata[4 * i + 3] = particles[i].color.w;
	}

}

void ParticleSpawner_Fire::Initialize(float width, float height, float capac)
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
		particles[i].position = glm::vec3(0, -1000, 0);
		particles[i].color = glm::vec4(0, 0, 0, 0);
	}

	for (int i = 0; i < capac; i++)
	{
		positiondata[3 * i + 0] = particles[i].position.x;
		positiondata[3 * i + 1] = particles[i].position.y;
		positiondata[3 * i + 2] = particles[i].position.z;

		colordata[4 * i + 0] = particles[i].color.x;
		colordata[4 * i + 1] = particles[i].color.y;
		colordata[4 * i + 2] = particles[i].color.z;
		colordata[4 * i + 3] = particles[i].color.w;
	}
}

void ParticleSpawner_Fire::Update()
{
	extern Camera camera;

	//Add new particles
	delaycounter++;
	if (delaycounter % delay == 0)
	{
		delaycounter = 0;
		for (int i = 0; i < spawnspeed; i++)
		{
			int unusedparticle = FirstUnusedParticle();
			if (unusedparticle >= particles.size())
			{
				cout << "wrong unusedindex" << endl;
				return;
			}
			Respawn(unusedparticle);
		}
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
				particles[i].color.z = (particles[i].life / (particlelife + (particlelife*.75f))) + .2f;
				particles[i].color.y = (particles[i].life / (particlelife + (particlelife*.75f))) + .6f;

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

void ParticleSpawner_Fire::Render(Shader* shader)
{
	extern glm::mat4 mMat;
	extern glm::mat4 vMat;
	extern glm::mat4 mvMat;
	extern glm::mat4 pMat;
	extern Camera camera;
	extern Shader shadermain;
	extern Texture firetexture;

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

	firetexture.UseTexture();
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

int ParticleSpawner_Fire::FirstUnusedParticle()
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

void ParticleSpawner_Fire::Respawn(int index)
{
	int randomx, randomz, randomlife;
	randomx = (rand() % xarea) - xarea;
	randomz = (rand() % zarea) - zarea;
	randomlife = rand() % (int)(particlelife*.75f) + particlelife;
	particles[index].position = glm::vec3( ((randomx / 100.0f)*10.0f) + particlecenter.x, particlecenter.y, ((randomz / 100.0f)*10.0f) + particlecenter.z);
	particles[index].velocity = glm::vec3(0, particlevelocity, 0);
	particles[index].color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
	particles[index].rotatematrix = Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), Utils::ToRadians(rand() % 360));
	particles[index].angle = Utils::ToRadians(rand() % 360);
	particles[index].life = randomlife;
}

ParticleSpawner_Fire::~ParticleSpawner_Fire()
{
	if(positiondata != nullptr)
		delete[] positiondata;
	if (colordata != nullptr)
		delete[] colordata;
	if (angledata != nullptr)
		delete[] angledata;
}
