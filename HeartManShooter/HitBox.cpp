#include "HitBox.h"



HitBox::HitBox()
{
	model = nullptr;
	center = glm::vec3(0, 0, 0);
	width = 0;
	height = 0;
	thedepth = 0;
}

HitBox::HitBox(glm::vec3 thecenter, GLfloat thewidth, GLfloat theheight, GLfloat depth, bool Rendered)
{
	center = thecenter;
	width = thewidth;
	height = theheight;
	thedepth = depth;

	if (Rendered)
	{
		model = new Rectangle;
		model->GenerateRectangleIndex(width, height, thedepth);
	}
}

void HitBox::Initialize(glm::vec3 thecenter, GLfloat thewidth, GLfloat theheight, GLfloat depth, bool Rendered)
{
	center = thecenter;
	width = thewidth;
	height = theheight;
	thedepth = depth;

	if (Rendered)
	{
		if (model != nullptr)
			delete model;
		model = new Rectangle;
		model->GenerateRectangleIndex(width, height, thedepth);
	}
}

void HitBox::Initialize(glm::vec3 ftl, glm::vec3 ftr, glm::vec3 fbl, glm::vec3 fbr,
	glm::vec3 btl, glm::vec3 btr, glm::vec3 bbl, glm::vec3 bbr)
{

}

void HitBox::GenerateNewModel()
{
	if (model != nullptr)
		delete model;

	model = new Rectangle;
	model->GenerateRectangleIndex(width, height, thedepth);
}

HitBox::~HitBox()
{
	if (model != nullptr)
		delete model;
}
