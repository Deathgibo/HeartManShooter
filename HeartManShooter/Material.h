#pragma once
#include "Shader.h"

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
public:
	Material();
	Material(glm::vec3 amb, glm::vec3 dif, glm::vec3 spec, float shine);

	void UseMaterial(Shader* shader);
	~Material();

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;//brightness of specular light [0,x]
	float shininess;//radius of specular light 0-256 good amount
private:

};

