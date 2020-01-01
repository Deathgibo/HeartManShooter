#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 dir, glm::vec3 col, float inten);

	void Calculateup();
	void UseDirLight(Shader* shader, int index);

	~DirectionalLight();

	glm::vec3 direction;
	glm::vec3 up;
private:
};

