#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();
	SpotLight(glm::vec3 dir, float cut, float proc, glm::vec3 pos, float con, float lin, float exp, glm::vec3 col, float inten);
	
	void Calculateup();
	void UseSpotLight(Shader* shader, int index);
	~SpotLight();

	glm::vec3 direction;
	glm::vec3 up;
	float cutoff, procEdge;
private:
};

