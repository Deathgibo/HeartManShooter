#pragma once
#include "Utils.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
using namespace std;

class ParticleMesh
{
public:
	ParticleMesh();
	
	virtual void LoadParticleIndex(int particlesize, float width, float height);
	virtual void RenderMesh(int particlecount);
	virtual void UpdateBind(int particlesize, GLfloat* positiondata, GLfloat* colordata, GLfloat* angledata);

	~ParticleMesh();

	int meshsize;
	GLuint vao, vbo, positionbuffer,colorbuffer,anglebuffer, ibo, trianglessize, indexsize;
	bool usingindexbuffer;

	int indexdatasize;
	int meshdatasize;

private:
};

