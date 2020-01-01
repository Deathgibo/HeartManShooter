#pragma once
#include "Rectangle.h"

class HitBox
{
public:
	HitBox();
	HitBox(glm::vec3 thecenter, GLfloat thewidth, GLfloat theheight, GLfloat depth, bool Rendered = false);
	void Initialize(glm::vec3 thecenter, GLfloat thewidth, GLfloat theheight, GLfloat depth, bool Rendered = false);
	void Initialize(glm::vec3 ftl, glm::vec3 ftr, glm::vec3 fbl, glm::vec3 fbr,
		glm::vec3 btl, glm::vec3 btr, glm::vec3 bbl, glm::vec3 bbr);
	void GenerateNewModel();
	~HitBox();

	glm::vec3 center;
	GLfloat width, height, thedepth;
	
	Rectangle* model;
private:

};

