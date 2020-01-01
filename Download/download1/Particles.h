#pragma once
#include <glm\glm.hpp>


class Particles
{
public:
	Particles();
	Particles(glm::vec3 pos, glm::vec3 vel, glm::vec4 col, int live);

	bool operator<(Particles& part);

	~Particles();

	glm::vec3 position, velocity;
	glm::vec4 color;
	glm::mat4 rotatematrix;
	int life;
	float angle;
	float cameradistance;
	//Square model;
	
private:

};

