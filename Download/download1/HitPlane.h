#pragma once
#include "HitBox.h"
#include "Plane.h"

class HitPlane
{
public:
	HitPlane();
	HitPlane(GLfloat thewidth, GLfloat theheight, GLfloat thedepth, bool Rendered = false);
	HitPlane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, bool Rendered = false, glm::vec3 thecenter = glm::vec3(0, 0, 0), int theangle = 0, bool HitBoxz = true);

	void Initialize(GLfloat thewidth, GLfloat theheight, GLfloat thedepth, bool Rendered = false);
	void GenerateNewModel();
	void Scale(float x, float y, float z);
	void Translate(float x, float y, float z);
	void Rotate(int angle);

	~HitPlane();

	glm::vec3 pointfl;
	glm::vec3 pointfr;
	glm::vec3 pointbl;
	glm::vec3 pointbr;
	glm::vec3 normal;
	glm::vec3 center;
	int angle;
	vector<HitBox> hitboxes;
	
	Plane *model;
private:
	void GenerateHitBoxes();
};

