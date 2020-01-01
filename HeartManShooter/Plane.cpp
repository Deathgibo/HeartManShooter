#include "Plane.h"

Plane::Plane()
{
	center = glm::vec3(0, 0, 0);
	angle = 0;
}

void Plane::CreatePlaneIndex(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4)
{
	float vertexPositions[32] = {
		p1.x,p1.y,p1.z,0,0,0,0,1,	//fl  0
		p2.x,p2.y,p2.z,0,0,0,1,1,	//fr  1
		p3.x,p3.y,p3.z,0,0,0,0,0,	//bl  2
		p4.x,p4.y,p4.z,0,0,0,1,0,	//br  3
	};

	unsigned int indices[6] = {
		0,2,1,
		1,2,3
	};

	Utils::calcAverageNormals(indices, 6, vertexPositions, 32, 8, 3);

	LoadMeshIndex(vertexPositions, 32, indices, 6);
}

void Plane::CreatePlaneIndex(float width, float height, float depth)
{
	thewidth = width; theheight = height; thedepth = depth;

	float vertexPositions[32] = {
		-width,0,depth,0,0,0,0,1,	//fl  0
		width,0,depth,0,0,0,1,1,	//fr  1
		-width,height,-depth,0,0,0,0,0,	//bl  2
		width,height,-depth,0,0,0,1,0,	//br  3
	};

	unsigned int indices[6] = {
		0,2,1,
		1,2,3
	};

	Utils::calcAverageNormals(indices, 6, vertexPositions, 32, 8, 3);

	LoadMeshIndex(vertexPositions, 32, indices, 6);
}

Plane::~Plane()
{
	if (meshdata != nullptr)
		delete meshdata;
	if (indexdata != nullptr)
		delete indexdata;
	meshdata = nullptr;
	indexdata = nullptr;
}
