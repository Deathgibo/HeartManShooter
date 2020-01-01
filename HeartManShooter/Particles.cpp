#include "Particles.h"

Particles::Particles()
{
	position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	color = glm::vec4(0, 0, 0,1);
	life = 0;
	rotatematrix = glm::mat4(1.0f);
	angle = 0;
}

Particles::Particles(glm::vec3 pos, glm::vec3 vel, glm::vec4 col, int live)
{
	position = pos;
	velocity = vel;
	color = col;
	life = live;
	rotatematrix = glm::mat4(1.0f);
}

bool Particles::operator<(Particles& part)
{
	return this->cameradistance > part.cameradistance;
}

Particles::~Particles()
{

}
