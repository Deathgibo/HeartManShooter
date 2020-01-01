#include "HitPlane.h"

HitPlane::HitPlane()
{
	pointfl = glm::vec3(0, 0, 0);
	pointfr = glm::vec3(0, 0, 0);
	pointbl = glm::vec3(0, 0, 0);
	pointbr = glm::vec3(0, 0, 0);
	normal = glm::vec3(0, 0, 0);
	center = glm::vec3(0, 0, 0);
	angle = 0;
	model = nullptr;
}

HitPlane::HitPlane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, bool Rendered, glm::vec3 thecenter, int theangle, bool HitBoxz)
{
	normal = glm::vec3(0, 0, 0);
	center = thecenter;
	angle = theangle;
	model = nullptr;

	pointfl = p1;
	pointfr = p2;
	pointbl = p3;
	pointbr = p4;

	if (Rendered)
	{
		if (model != nullptr)
			delete model;
		model = new Plane();
		model->CreatePlaneIndex(pointfl, pointfr, pointbl, pointbr);
	}

	normal = Utils::CalculateNormal(pointfl, pointbl, pointfr);
	if(HitBoxz)
		GenerateHitBoxes();
}

HitPlane::HitPlane(GLfloat thewidth, GLfloat theheight, GLfloat thedepth, bool Rendered)
{
	normal = glm::vec3(0, 0, 0);
	center = glm::vec3(0, 0, 0);
	angle = 0;
	model = nullptr;

	pointfl = glm::vec3(-thewidth, 0, 0);
	pointfr = glm::vec3(thewidth, 0, 0);
	pointbl = glm::vec3(-thewidth, theheight, -thedepth);
	pointbr = glm::vec3(thewidth, theheight, -thedepth);

	if (Rendered)
	{
		if (model != nullptr)
			delete model;
		model = new Plane();
		model->CreatePlaneIndex(pointfl, pointfr, pointbl, pointbr);
	}

	normal = Utils::CalculateNormal(pointfl, pointbl, pointfr);
	GenerateHitBoxes();
}

void HitPlane::Initialize(GLfloat thewidth, GLfloat theheight, GLfloat thedepth, bool Rendered)
{
	pointfl = glm::vec3(-thewidth,0, 0);
	pointfr = glm::vec3(thewidth,0, 0);
	pointbl = glm::vec3(-thewidth,theheight,-thedepth);
	pointbr = glm::vec3(thewidth,theheight,-thedepth);

	if (Rendered)
	{
		if (model != nullptr)
			delete model;
		model = new Plane();
		model->CreatePlaneIndex(pointfl, pointfr, pointbl, pointbr);
	}

	normal = Utils::CalculateNormal(pointfl, pointbl, pointfr);
	GenerateHitBoxes();
}

void HitPlane::GenerateNewModel()
{
	if (model != nullptr)
		delete model;
	model = new Plane();
	model->CreatePlaneIndex(pointfl, pointfr, pointbl, pointbr);
	GenerateHitBoxes();
}

void HitPlane::Scale(float x, float y, float z)
{
	int holdangle = angle;
	Rotate(-angle);
	pointfl.x -= x; pointfl.y += 0; pointfl.z += 0;
	pointfr.x += x; pointfr.y += 0; pointfr.z += 0;
	pointbl.x -= x; pointbl.y += y; pointbl.z -= z;
	pointbr.x += x; pointbr.y += y; pointbr.z -= z;
	Rotate(holdangle);
}

void HitPlane::Translate(float x, float y, float z)
{
	center.x += x; center.y += y; center.z += z;
	
	pointfl.x += x; pointfl.y += y; pointfl.z += z;
	pointfr.x += x; pointfr.y += y; pointfr.z += z;
	pointbl.x += x; pointbl.y += y; pointbl.z += z;
	pointbr.x += x; pointbr.y += y; pointbr.z += z;
}

void HitPlane::Rotate(int theangle)
{
	angle += theangle;
	angle = angle % 360;

	glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), Utils::ToRadians(theangle), glm::vec3(0, 1.0f, 0));
	glm::mat3 thematrix = matrix;

	pointfl.x -= center.x; pointfl.y -= center.y; pointfl.z -= center.z;
	pointfr.x -= center.x; pointfr.y -= center.y; pointfr.z -= center.z;
	pointbl.x -= center.x; pointbl.y -= center.y; pointbl.z -= center.z;
	pointbr.x -= center.x; pointbr.y -= center.y; pointbr.z -= center.z;

	pointfl = thematrix * pointfl;
	pointfr = thematrix * pointfr;
	pointbl = thematrix * pointbl;
	pointbr = thematrix * pointbr;

	pointfl.x += center.x; pointfl.y += center.y; pointfl.z += center.z;
	pointfr.x += center.x; pointfr.y += center.y; pointfr.z += center.z;
	pointbl.x += center.x; pointbl.y += center.y; pointbl.z += center.z;
	pointbr.x += center.x; pointbr.y += center.y; pointbr.z += center.z;
}

void HitPlane::GenerateHitBoxes()
{
	glm::vec3 tmpcenter(0, 0, 0);
	float tmpwidth = 0; float tmpheight = 0; float tmpdepth = 0;
	float percenty = .20f;
	float ylength = pointbl.y - pointfl.y;
	glm::vec3 direction = pointbl - pointfl;
	direction = glm::normalize(direction);
	hitboxes.clear();
	if (direction.y == 0.0f)
	{
		//cout << "Hitplane is flat cant generate rects..." << endl;
		return;
	}
	for(float percent = percenty;percent < 1.0f;percent+=percenty)
	{
		float multiple = ((pointfl.y + ylength*percent) - pointfl.y) / (direction.y);
		glm::vec3 left = pointfl + (multiple * direction);
		glm::vec3 right = pointfr + (multiple * direction);

		//create hitbox with two points find center,width,height,depth *only two types either left or right have same x or z
		if(angle == 90 || angle == 270)
		{
			tmpcenter.x = ((left.x + pointbl.x) / 2.0f);
			tmpcenter.y = (pointfl.y + ylength * (percent - (percenty / 2.0f)));
			tmpcenter.z = ((right.z + left.z) / 2.0f);
			tmpwidth = abs(tmpcenter.x - left.x);
			tmpdepth = abs(tmpcenter.z - left.z);
			tmpheight = ylength * (percenty / 2.0f);
		}
		else if(angle == 0 || angle == 180)
		{
			tmpcenter.x = ((right.x+left.x)/2.0f);
			tmpcenter.y = (pointfl.y + ylength*(percent - (percenty/2.0f)));
			tmpcenter.z = ((left.z + pointbl.z)/2.0f);
			tmpwidth = abs(tmpcenter.x - left.x);
			tmpdepth = abs(tmpcenter.z - left.z);
			tmpheight = ylength * (percenty/2.0f);
		}
		else
		{
			cout << "hitplane: two points dont share same x or z error..." << endl;
			return;
		}
	

		if (hitboxes.size() >= hitboxes.capacity())
		{
			hitboxes.reserve(hitboxes.size() + 5);
		}
		hitboxes.emplace_back(tmpcenter, tmpwidth, tmpheight, tmpdepth, true);
		
	}
	
}

HitPlane::~HitPlane()
{
	if (model != nullptr)
		delete model;

	hitboxes.clear();
}
