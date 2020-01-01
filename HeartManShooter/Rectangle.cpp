#include "Rectangle.h"

Rectangle::Rectangle()
{

}

Rectangle::Rectangle(float thewidth, float theheight, glm::vec3 thecenter)
{
	GenerateFlatRectangle(thewidth, theheight);
	center = thecenter;
}

Rectangle::Rectangle(glm::vec3 norm, glm::vec3 left, glm::vec3 up, glm::vec3 acenter, float thewidth, float theheight, float thedepth)
{
	GenerateFlatRectangleInTriangle(norm, left, up, acenter, thewidth, theheight);
	center = acenter;
}

Rectangle::~Rectangle()
{

}

void Rectangle::GenerateFlatRectangleInTriangle(glm::vec3 norm, glm::vec3 left, glm::vec3 up, glm::vec3 acenter, float thewidth, float theheight, float thedepth)
{
	width = thewidth;
	height = theheight;

	//center - left*thewidth/2
	//center + left*halfwidth
	float halfwidth = thewidth / 2.f;
	float halfheight = theheight / 2.f;

	glm::vec3 tl = -(left*halfwidth) + (up*halfheight);
	glm::vec3 tr = (left*halfwidth) + (up*halfheight);
	glm::vec3 bl = -(left*halfwidth) - (up*halfheight);
	glm::vec3 br = (left*halfwidth) - (up*halfheight);

	float vertexPositions[64] = {
		tl.x,tl.y,tl.z,0,0,0,0,1,		//ftl  0
		tl.x,tl.y,tl.z,0,0,0,0,0,		//btl  1
		bl.x,bl.y,bl.z,0,0,0,0,0,		//fbl  2
		bl.x,bl.y,bl.z,0,0,0,0,0,		//bbl  3

		tr.x,tr.y,tr.z,0,0,0,1,1,		//ftr  4
		tr.x,tr.y,tr.z,0,0,0,0,0,		//btr  5
		br.x,br.y,br.z,0,0,0,1,0,		//fbr  6
		br.x,br.y,br.z,0,0,0,0,0		//bbr  7
	};

	unsigned int indices[36] = {
		0,4,2,
		4,6,2,
		4,5,6,
		5,7,6,

		5,3,7,
		3,5,1,

		1,2,3,
		0,2,1,

		0,1,4,
		5,4,1,

		7,3,2,
		6,7,2
	};

	Utils::calcAverageNormals(indices, 36, vertexPositions, 64, 8, 3);

	vector<float> newbuffer;
	for (int i = 0; i < 36; i++)
	{
		newbuffer.insert(newbuffer.end(), { vertexPositions[indices[i] * 8],vertexPositions[indices[i] * 8 + 1], vertexPositions[indices[i] * 8 + 2],
			vertexPositions[indices[i] * 8 + 3], vertexPositions[indices[i] * 8 + 4], vertexPositions[indices[i] * 8 + 5], vertexPositions[indices[i] * 8 + 6],
			vertexPositions[indices[i] * 8 + 7] });
	}

	LoadMesh(&newbuffer[0], 288);
}

void Rectangle::GenerateFlatRectangle(float thewidth, float theheight, float thedepth)
{
	width = thewidth;
	height = theheight;

	float vertexPositions[64] = {
		-thewidth,theheight,thedepth,0,0,0,0,1,		//ftl  0
		-thewidth,theheight,-thedepth,0,0,0,0,0,	//btl  1
		-thewidth,-theheight,thedepth,0,0,0,0,0,	//fbl  2
		-thewidth,-theheight,-thedepth,0,0,0,0,0,	//bbl  3

		thewidth,theheight,thedepth,0,0,0,1,1,		//ftr  4
		thewidth,theheight,-thedepth,0,0,0,0,0,		//btr  5
		thewidth,-theheight,thedepth,0,0,0,1,0,		//fbr  6
		thewidth,-theheight,-thedepth,0,0,0,0,0		//bbr  7
	};



	unsigned int indices[36] = {
		0,4,2,
		4,6,2,
		4,5,6,
		5,7,6,

		5,3,7,
		3,5,1,

		1,2,3,
		0,2,1,

		0,1,4,
		5,4,1,

		7,3,2,
		6,7,2
	};

	Utils::calcAverageNormals(indices, 36, vertexPositions, 64, 8, 3);

	vector<float> newbuffer;
	for (int i = 0; i < 36; i++)
	{
		newbuffer.insert(newbuffer.end(), { vertexPositions[indices[i] * 8],vertexPositions[indices[i] * 8 + 1], vertexPositions[indices[i] * 8 + 2],
			vertexPositions[indices[i] * 8 + 3], vertexPositions[indices[i] * 8 + 4], vertexPositions[indices[i] * 8 + 5], vertexPositions[indices[i] * 8 + 6],
			vertexPositions[indices[i] * 8 + 7] });
	}

	LoadMesh(&newbuffer[0], 288);

}

void Rectangle::GenerateRectangleskybox(float thewidth, float theheight, float thedepth)
{
	width = thewidth;
	height = theheight;

	float vertexPositions[64] = {
		-thewidth,theheight,thedepth,0,0,0,0,1,		//ftl  0
		-thewidth,theheight,-thedepth,0,0,0,1,1,	//btl  1
		-thewidth,-theheight,thedepth,0,0,0,0,0,	//fbl  2
		-thewidth,-theheight,-thedepth,0,0,0,1,0,	//bbl  3

		thewidth,theheight,thedepth,0,0,0,1,1,		//ftr  4
		thewidth,theheight,-thedepth,0,0,0,0,1,		//btr  5
		thewidth,-theheight,thedepth,0,0,0,1,0,		//fbr  6
		thewidth,-theheight,-thedepth,0,0,0,0,0		//bbr  7
	};

	unsigned int indices[36] = {
		0,4,2,
		4,6,2,
		4,5,6,
		5,7,6,

		5,3,7,
		3,5,1,

		1,2,3,
		0,2,1,

		0,1,4,
		5,4,1,

		7,3,2,
		6,7,2
	};

	Utils::calcAverageNormals(indices, 36, vertexPositions, 64, 8, 3);
	cout << vertexPositions[3] << " " << vertexPositions[4] << " " << vertexPositions[5] << endl;
	vector<float> newbuffer;
	for (int i = 0; i < 36; i++)
	{
		newbuffer.insert(newbuffer.end(), { vertexPositions[indices[i] * 8],vertexPositions[indices[i] * 8 + 1], vertexPositions[indices[i] * 8 + 2],
			vertexPositions[indices[i] * 8 + 3], vertexPositions[indices[i] * 8 + 4], vertexPositions[indices[i] * 8 + 5], vertexPositions[indices[i] * 8 + 6],
			vertexPositions[indices[i] * 8 + 7] });
	}

	LoadMesh(&newbuffer[0], 288);
}

void Rectangle::GenerateRectangle(float thewidth, float theheight, float thedepth)
{
	width = thewidth;
	height = theheight;
	
	float vertexPositions[64] = {
		-thewidth,theheight,thedepth,0,0,0,0,1,		//ftl  0
		-thewidth,theheight,-thedepth,0,0,0,1,1,	//btl  1
		-thewidth,-theheight,thedepth,0,0,0,0,0,	//fbl  2
		-thewidth,-theheight,-thedepth,0,0,0,1,0,	//bbl  3

		thewidth,theheight,thedepth,0,0,0,1,1,		//ftr  4
		thewidth,theheight,-thedepth,0,0,0,0,1,		//btr  5
		thewidth,-theheight,thedepth,0,0,0,1,0,		//fbr  6
		thewidth,-theheight,-thedepth,0,0,0,0,0		//bbr  7
	};

	unsigned int indices[36] = {
		0,4,2,
		4,6,2,
		4,5,6,
		5,7,6,
		
		5,3,7,
		3,5,1,
		
		1,2,3,
		0,2,1,
		
		0,1,4,
		5,4,1,
		
		7,3,2,
		6,7,2
	};

	Utils::calcAverageNormals(indices, 36, vertexPositions, 64, 8, 3);
	cout << vertexPositions[3] << " " << vertexPositions[4] << " " << vertexPositions[5] << endl;
	vector<float> newbuffer;
	for (int i = 0; i < 36; i ++)
	{
		newbuffer.insert(newbuffer.end(), { vertexPositions[indices[i]*8],vertexPositions[indices[i] * 8 + 1], vertexPositions[indices[i] * 8 + 2], 
			vertexPositions[indices[i] * 8 + 3], vertexPositions[indices[i] * 8 + 4], vertexPositions[indices[i] * 8 + 5], vertexPositions[indices[i] * 8 + 6],
			vertexPositions[indices[i] * 8 + 7] });
	}
/*	int index = 0;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 1;
	index += 8;
	
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 1;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 1;
	index += 8;
	
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 1;
	index += 8;
	
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 1;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 1;
	index += 8;
	
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 1;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 1;
	index += 8;
	
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 1;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 0;
	index += 8;

	newbuffer[index + 6] = 0; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 1;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 1;
	index += 8;

	newbuffer[index + 6] = 1; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 1;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 0;
	index += 8;

	newbuffer[index + 6] = 0; newbuffer[index + 7] = 1;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 1;
	index += 8;

	newbuffer[index + 6] = 1; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 1;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 0;
	index += 8;
	
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 1;
	index += 8;
	
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 1;
	index += 8;
	newbuffer[index + 6] = 1; newbuffer[index + 7] = 0;
	index += 8;
	newbuffer[index + 6] = 0; newbuffer[index + 7] = 1;*/
	
	vector<float> vertexpositions;

	/*for (int i = 0; i < 36; i++)
	{
		int index = indices[i] * 8;
		for (int x = 0; x < 8; x++)
		{
			//cout << vertexPositions[index + 3] << " " << vertexPositions[index + 4] << " " << vertexpositions[index + 5] << endl;
			vertexpositions.push_back(vertexPositions[index + x]);
		}
	}*/

	//set up text coords

	LoadMesh(&newbuffer[0], 288);

	planes[0].point = glm::vec3(-thewidth, theheight, thedepth);
	planes[0].normal = glm::vec3(0, 0, -1);

	planes[1].point = glm::vec3(thewidth, theheight, thedepth);
	planes[1].normal = glm::vec3(-1, 0, 0);

	planes[2].point = glm::vec3(-thewidth, -theheight, -thedepth);
	planes[2].normal = glm::vec3(0, 0, 1);

	planes[3].point = glm::vec3(-thewidth, -theheight, -thedepth);
	planes[3].normal = glm::vec3(1, 0, 0);

	planes[4].point = glm::vec3(-thewidth, theheight, thedepth);
	planes[4].normal = glm::vec3(0, -1, 0);

	planes[5].point = glm::vec3(-thewidth, -theheight, -thedepth);
	planes[5].normal = glm::vec3(0, 1, 0);
}

void Rectangle::GenerateRectangleIndex(float thewidth, float theheight, float thedepth)
{
	width = thewidth;
	height = theheight;

	float vertexPositions[64] = {
		-thewidth,theheight,thedepth,0,0,0,0,1,		//ftl  0
		-thewidth,theheight,-thedepth,0,0,0,1,1,	//btl  1
		-thewidth,-theheight,thedepth,0,0,0,0,0,	//fbl  2
		-thewidth,-theheight,-thedepth,0,0,0,1,0,	//bbl  3
		
		thewidth,theheight,thedepth,0,0,0,1,1,		//ftr  4
		thewidth,theheight,-thedepth,0,0,0,0,1,		//btr  5
		thewidth,-theheight,thedepth,0,0,0,1,0,		//fbr  6
		thewidth,-theheight,-thedepth,0,0,0,0,0		//bbr  7
	};

	unsigned int indices[36] = {
		0,4,2,
		4,6,2,
		4,5,6,
		5,7,6,
		5,3,7,
		3,5,1,
		1,2,3,
		0,2,1,
		0,1,4,
		5,4,1,
		7,3,2,
		6,7,2
	};
	Utils::calcAverageNormals(indices, 36, vertexPositions, 64, 8, 3);
	LoadMeshIndex(vertexPositions, 64, indices, 36);

	planes[0].point =  glm::vec3(-thewidth, theheight, thedepth);
	planes[0].normal = glm::vec3(0,0,-1);

	planes[1].point = glm::vec3(thewidth, theheight, thedepth);
	planes[1].normal = glm::vec3(-1, 0, 0);

	planes[2].point = glm::vec3(-thewidth, -theheight, -thedepth);
	planes[2].normal = glm::vec3(0, 0, 1);

	planes[3].point = glm::vec3(-thewidth, -theheight, -thedepth);
	planes[3].normal = glm::vec3(1, 0, 0);

	planes[4].point = glm::vec3(-thewidth, theheight, thedepth);
	planes[4].normal = glm::vec3(0, -1, 0);

	planes[5].point = glm::vec3(-thewidth, -theheight, -thedepth);
	planes[5].normal = glm::vec3(0, 1, 0);
}

void Rectangle::GenerateRectangleIndex(glm::vec3 ftl, glm::vec3 ftr, glm::vec3 fbl, glm::vec3 fbr,
	glm::vec3 btl, glm::vec3 btr, glm::vec3 bbl, glm::vec3 bbr)
{
	//width = thewidth;
	//height = theheight;

	float vertexPositions[64] = {
		ftl.x,ftl.y,ftl.z,0,0,0,0,1,		//ftl  0
		btl.x,btl.y,btl.z,0,0,0,1,1,	//btl  1
		fbl.x,fbl.y,fbl.z,0,0,0,0,0,	//fbl  2
		bbl.x,bbl.y,bbl.z,0,0,0,1,0,	//bbl  3

		ftr.x,ftr.y,ftr.z,0,0,0,1,1,		//ftr  4
		btr.x,btr.y,btr.z,0,0,0,0,1,		//btr  5
		fbr.x,fbr.y,fbr.z,0,0,0,1,0,		//fbr  6
		bbr.x,bbr.y,bbr.z,0,0,0,0,0		//bbr  7
	};

	unsigned int indices[36] = {
		0,4,2,
		4,6,2,
		4,5,6,
		5,7,6,
		5,3,7,
		3,5,1,
		1,2,3,
		0,2,1,
		0,1,4,
		5,4,1,
		7,3,2,
		6,7,2
	};
	Utils::calcAverageNormals(indices, 36, vertexPositions, 64, 8, 3);
	LoadMeshIndex(vertexPositions, 64, indices, 36);

}