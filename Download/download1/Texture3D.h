#pragma once

#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <SOIL2.h>
#include "stb_image.h"

#include "Utils.h"

class Texture3D
{
public:
	Texture3D();

	void generate3dpattern();
	void filldataarrayflash(GLubyte data[]);
	void filldataarray(GLubyte data[]);
	void load3dtexture();

	void UseTexture3d();

	~Texture3D();

	GLuint textureid;
	const int texwidth = 10;
	const int texheight = 10;
	const int texdepth = 10;
	double tex3dpattern[200][200][200];
private:

};

