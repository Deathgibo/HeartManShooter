#include "Utils.h"
#include "Sphere.h"
#include "Rectangle.h"
#include "HitPlane.h"

bool Utils::checkOpenGLError()
{
	bool errorfound = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		std::cout << "glError: " << glErr << std::endl;
		errorfound = true;
		glErr = glGetError();
	}
	return errorfound;
}

void Utils::calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		//std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}
	for (int i = 9; i < 48; i += 12)
	{
		//std::cout << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << std::endl;
	}
	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

glm::mat4 Utils::GetRodriguezMatrix(glm::vec3 n, float angle)
{
	glm::mat4 matrix(1.0f);

	matrix[0][0] = cos(angle) + (n.x*n.x)*(1-cos(angle));		matrix[1][0] = n.x*n.y*(1-cos(angle)) - n.z*sin(angle);		matrix[2][0] = n.x*n.z*(1-cos(angle))+n.y*sin(angle);		matrix[3][0] = 0;
	matrix[0][1] = n.y*n.x*(1 - cos(angle)) + n.z*sin(angle);	matrix[1][1] = cos(angle) + n.y*n.y*(1-cos(angle));			matrix[2][1] = n.y*n.z*(1-cos(angle)) - n.x*sin(angle);		matrix[3][1] = 0;
	matrix[0][2] = n.z*n.x*(1-cos(angle)) - n.y*sin(angle);		matrix[1][2] = n.z*n.y*(1-cos(angle)) + n.x*sin(angle);		matrix[2][2] = cos(angle) + n.z*n.z*(1-cos(angle));			matrix[3][2] = 0;
	matrix[0][3] = 0;											matrix[1][3] = 0;											matrix[2][3] = 0;											matrix[3][3] = 1;

	return matrix;
}

bool Utils::boxboxcollision(const HitBox& a, const HitBox& b)
{

	//The sides of the rectangles
	float leftA, leftB;
	float rightA, rightB;
	float topA, topB;
	float bottomA, bottomB;
	float frontB, backB;
	float frontA, backA;

	//Calculate the sides of rect A
	leftA = a.center.x - a.width;
	rightA = a.center.x + a.width;
	topA = a.center.y + a.height;
	bottomA = a.center.y - a.height;
	frontA = a.center.z + a.thedepth;
	backA = a.center.z - a.thedepth;

	//Calculate the sides of rect B
	leftB = b.center.x - b.width;
	rightB = b.center.x + b.width;
	topB = b.center.y + b.height;
	bottomB = b.center.y - b.height;
	frontB = b.center.z + b.thedepth;
	backB = b.center.z - b.thedepth;

	//cout << frontA << " " << backA << " " << frontB << " " << backB << endl;
	//If any of the sides from A are outside of B
	if (bottomA >= topB)
	{
		return false;
	}

	if (topA <= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	if (backA >= frontB)
	{
		return false;
	}

	if (frontA <= backB)
	{
		return false;
	}

	return true;
}

bool Utils::planeplanecollision(const HitPlane& a, const HitPlane& b)
{
	//if feet y is not in y range no collision
	if (a.pointfl.y < b.pointfl.y || a.pointfl.y > b.pointbl.y)
	{
		return false;
	}
	//find left and right point at certain y
	glm::vec3 direction = b.pointbl - b.pointfl;
	direction = glm::normalize(direction);
	float multiple = (a.pointfl.y - b.pointfl.y) / (direction.y);
	glm::vec3 left = b.pointfl + (multiple * direction);
	glm::vec3 right = b.pointfr + (multiple * direction);

	//**if slope is really small function might just miss collision completely maybe fix or just make sure slopes arent like flat
	if (segmentboxcollision(left, right, a.pointfl, a.pointfr, a.pointbl, a.pointfr))
	{
		return true;
	}

	return false;
}

bool Utils::segmentboxcollision(glm::vec3 p1, glm::vec3 p2, glm::vec3 bleft, glm::vec3 bright, glm::vec3 bback, glm::vec3 bfront)
{
	float boxminx = bleft.x;
	float boxmaxx = bright.x;
	float boxminz = bback.z;
	float boxmaxz = bfront.z;

	float segminx, segmaxx, segminz, segmaxz;
	if (p1.x < p2.x) {
		segminx = p1.x; segmaxx = p2.x;
	}
	else{ 
		segminx = p2.x; segmaxx = p1.x; 
	}
	if (p1.z < p2.z) {
		segminz = p1.z; segmaxz = p2.z;
	}
	else {
		segminz = p2.z; segmaxz = p1.z;
	}
	
	if (boxmaxx < segminx)
	{
		return false;
	}

	if (boxminx > segmaxx)
	{
		return false;
	}
	if (boxmaxz < segminz)
	{
		return false;
	}

	if (boxminz > segmaxz)
	{
		return false;
	}
	return true;
}

float Utils::ToRadians(float degree)
{
	return (degree * 3.14) / 180;
}

float Utils::Distance3vec(glm::vec3 a, glm::vec3 b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

float Utils::RayTriCollision(Ray r, Triangle tri)
{
	//compute scalar on 3rd tri vertex (y)
	//std::cout << r.getdirection().z << std::endl;
	float tt = RayTriHelper(2, r, tri);
	//std::cout << tt << std::endl;
	if (tt < 0 || tt > 10000)
	{
		return -1;
	}
	//ts = tt;
	float y = RayTriHelper(1, r, tri);
	//cout << y << endl;
	if (y < 0 || y > 1)
	{
		return -1;
	}

	//computer scalar on 2nd tri vertex(b)
	float b = RayTriHelper(0, r, tri);

	if (b < 0 || b > 1 - y)
	{
		return -1;
	}

	return tt;
}

float Utils::RayTriHelper(int y, Ray r, Triangle tri)
{
	float a, b, c, d, e, f, g, h, i, j, k, l;
	a = tri.one.x - tri.two.x;
	b = tri.one.y - tri.two.y;
	c = tri.one.z - tri.two.z;
	d = tri.one.x - tri.three.x;
	e = tri.one.y - tri.three.y;
	f = tri.one.z - tri.three.z;
	g = r.direction.x;
	h = r.direction.y;
	i = r.direction.z;
	j = tri.one.x - r.position.x;
	k = tri.one.y - r.position.y;
	l = tri.one.z - r.position.z;
	//regular determinent of raypointposition = trianglepositions
	float m = a * (e*i - h * f) + b * (g*f - d * i) + c * (d*h - e * g);
	float they;
	if (y == 1)
	{
		they = i * (a*k - j * b) + h * (j*c - a * l) + g * (b*l - k * c);
	}
	else if (y == 0)
	{
		they = j * (e*i - h * f) + k * (g*f - d * i) + l * (d*h - e * g);
	}
	else
	{
		they = f * (a*k - j * b) + e * (j*c - a * l) + d * (b*l - k * c);
		they = -(they / m);
		return they;
	}
	they = (they / m);
	return they;
}

glm::vec3 Utils::CalculateNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;
	glm::vec3 normal = glm::cross(v1, v2);
	normal = glm::normalize(normal);

	return normal;
}