#include "Square.h"



Square::Square()
{
}

void Square::createsquare(float thewidth, float theheight)
{
	float vertexPositions[32] = {
		-thewidth,theheight,0,0,0,0,0,1,	//tl  0
		-thewidth,-theheight,0,0,0,0,0,0,	//bl  1
		thewidth,theheight,0,0,0,0,1,1,		//tr  2
		thewidth,-theheight,0,0,0,0,1,0		//br  3
	};

	unsigned int indices[6] = {
		0,2,1,
		2,3,1
	};

	Utils::calcAverageNormals(indices, 6, vertexPositions, 32, 8, 3);

	LoadMeshIndex(vertexPositions, 32, indices, 6);
}

void Square::createsquareflat(float thewidth, float thedepth)
{
	float vertexPositions[32] = {
		-thewidth,0,thedepth,0,0,0,0,1,	//tl  0
		-thewidth,0,-thedepth,0,0,0,0,0,	//bl  1
		thewidth,0,thedepth,0,0,0,1,1,		//tr  2
		thewidth,0,-thedepth,0,0,0,1,0		//br  3
	};

	unsigned int indices[6] = {
		0,2,1,
		2,3,1
	};

	Utils::calcAverageNormals(indices, 6, vertexPositions, 32, 8, 3);

	LoadMeshIndex(vertexPositions, 32, indices, 6);
}

void Square::createsquarenox(float theheight, float thedepth)
{
	float vertexPositions[32] = {
		0,theheight,thedepth,0,0,0,0,1,	//tl  0
		0,-theheight,thedepth,0,0,0,0,0,	//bl  1
		0,theheight,-thedepth,0,0,0,1,1,		//tr  2
		0,-theheight,-thedepth,0,0,0,1,0		//br  3
	};

	unsigned int indices[6] = {
		0,2,1,
		2,3,1
	};

	Utils::calcAverageNormals(indices, 6, vertexPositions, 32, 8, 3);

	LoadMeshIndex(vertexPositions, 32, indices, 6);
}

Square::~Square()
{
}
