#pragma once
#include "Mesh.h"

class Plane : public Mesh
{
public:
	Plane();

	void CreatePlaneIndex(float width, float height, float depth);
	void CreatePlaneIndex(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);

	~Plane();

	glm::vec3 center;
	float angle;
	float thewidth, theheight, thedepth;
private:

};

