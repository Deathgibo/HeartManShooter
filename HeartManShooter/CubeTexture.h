#pragma once
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2.h>
#include "stb_image.h"

class CubeTexture
{
public:
	CubeTexture();

	void LoadTextureA(std::vector<std::string> faceLocations);
	void LoadTexture(std::vector<std::string> faceLocations);
	void UseTexture(int index);

	~CubeTexture();

private:
	GLuint textureid;
	GLsizei width, height, bitDepth;
};

