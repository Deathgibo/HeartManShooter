#pragma once
#include <glm\glm.hpp>

class Ray {

public:
	Ray()
	{

	}

	void setray(float px, float py, float pz, float dx, float dy, float dz)
	{
		position.x = px; position.y = py; position.z = pz; 
		direction.x = dx; direction.y = dy; direction.z = dz;
	}

	void setray(glm::vec3 rayposition, glm::vec3 raydirection)
	{
		position = rayposition;
		direction = raydirection;
	}

	glm::vec3 position;
	glm::vec3 direction; //normalized
private:

};