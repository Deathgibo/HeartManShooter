#pragma once

#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <SOIL2.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* teximagepath);

	bool LoadTextureA();
	bool LoadTexture();

	void UseTexture();

	~Texture();
	int lol;
	const char* fileLocation;
	GLuint textureid;
	GLsizei width, height, bitDepth;

private:
};



