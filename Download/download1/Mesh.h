#pragma once
#include "Utils.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
using namespace std;

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh& obj);
	~Mesh();
	virtual void LoadMesh(GLfloat * data, int size, bool stillusingindex = false);
	virtual void LoadMeshIndex(GLfloat * data, int size, unsigned int* indices, int theindexsize);
	virtual void RenderMesh();
	virtual void UpdatePhysics(float percent);
	virtual void ReBindData(GLfloat * data, int size);
	virtual void ChangeUplodaData(glm::mat4 matrix, bool scalenormal = false);
	virtual void SetNormals();

	int meshsize;
	GLuint vao, vbo, ibo, trianglessize, indexsize;
	bool usingindexbuffer;
	GLfloat* meshdata;
	unsigned int* indexdata;
	int indexdatasize;
	int meshdatasize;

	glm::vec3 physicscenter;
	glm::vec3 velocity;
private:

};

