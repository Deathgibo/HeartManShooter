#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(glm::vec3 pos, float con, float lin, float exp, glm::vec3 col, float inten);

	void Initialize(glm::vec3 pos, float con, float lin, float exp, glm::vec3 col, float inten);
	void UsePointLight(Shader* shader, int index);

	~PointLight();

	glm::vec3 position;
	float constant, linear, exponent;
private:
};

