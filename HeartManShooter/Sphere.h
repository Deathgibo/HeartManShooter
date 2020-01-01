#pragma once
#include "Mesh.h"
class Sphere :
	public Mesh
{
public:
	Sphere();

	void GenerateSphere(float sectorCount, float stackCount, float theradius);
	void Move(float x, float y, float z);

	GLfloat radius;
	glm::vec3 center;
};

