#pragma once

#include "Shader.h"
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 col, float inten);
	
	void UseLight(Shader* shader);
	
	~Light();

	glm::vec3 color;
	float intensity;
private:
};

