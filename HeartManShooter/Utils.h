#pragma once
#include <GL\glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <stdio.h> 
#include <stdlib.h> 
#include<time.h> 

#include "Mesh.h"
#include "Ray.h"
#include "Triangle.h"
class Sphere;
class Rectangle;
class HitBox;
class HitPlane;
using namespace std;

struct plane {
	glm::vec3 point;
	glm::vec3 normal;
};

struct Singlekeys {
	bool key;
	bool down;
};

class Utils
{
public:
	static bool checkOpenGLError();
	static void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
		unsigned int vLength, unsigned int normalOffset);

	static glm::mat4 GetRodriguezMatrix(glm::vec3 a, float angle);

	static bool boxboxcollision(const HitBox& a, const HitBox& b);

	static bool planeplanecollision(const HitPlane& a, const HitPlane& b);

	static bool segmentboxcollision(glm::vec3 p1, glm::vec3 p2, glm::vec3 bleft, glm::vec3 bright, glm::vec3 bback, glm::vec3 bfront);
	
	static float ToRadians(float degree);

	static void SetRandomSeed() { srand(time(0)); }

	static float Distance3vec(glm::vec3 a, glm::vec3 b);

	static float RayTriCollision(Ray r, Triangle tri);

	static float RayTriHelper(int y, Ray r, Triangle tri);

	static glm::vec3 CalculateNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
private:

};