#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <stdio.h>
#include "Square.h"

class Water
{
public:
	Water();

	void CreateWater(float widthz, float heightz);
	void CreateWaterMesh(float widthz, float heightz);
	void Update();

	~Water();

	GLuint fboreflect, fborefract, colormapreflect, colormaprefract, depthmapreflect, depthmaprefract;
	float width, height;
	float meshheight;
	float wavespeed;
	float movefactor;
	Square mesh;
private:

};

