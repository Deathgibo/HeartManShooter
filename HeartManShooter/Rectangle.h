#pragma once
#include "Mesh.h"
#include "Utils.h"


class Rectangle :
	public Mesh
{
public:
	Rectangle();
	Rectangle(float thewidth, float theheight, glm::vec3 thecenter);
	Rectangle(glm::vec3 norm, glm::vec3 left, glm::vec3 up, glm::vec3 acenter, float thewidth, float theheight, float thedepth = 0);
	~Rectangle();

	void GenerateFlatRectangleInTriangle(glm::vec3 norm, glm::vec3 left, glm::vec3 up, glm::vec3 acenter, float thewidth, float theheight, float thedepth = 0);
	void GenerateFlatRectangle(float thewidth, float theheight, float thedepth = 0);
	void GenerateRectangleIndex(float thewidth, float theheight, float thedepth);
	void GenerateRectangle(float thewidth, float theheight, float thedepth);
	void GenerateRectangleskybox(float thewidth, float theheight, float thedepth);
	void GenerateRectangleIndex(glm::vec3 ftl, glm::vec3 ftr, glm::vec3 fbl, glm::vec3 fbr,
		glm::vec3 btl, glm::vec3 btr, glm::vec3 bbl, glm::vec3 bbr);

	glm::vec3 center;
	GLfloat width, height, thedepth;
	plane planes[6];
};

