#pragma once
#include <glm\glm.hpp>

class Triangle
{
public:
	Triangle() { ; };

	void SetTriangle(float onex, float oney, float onez, float twox, float twoy, float twoz, float threex, float threey, float threez)
	{
		one.x = onex; one.y = oney; one.z = onez;
		two.x = twox; two.y = twoy; two.z = twoz;
		three.x = threex; three.y = threey; three.z = threez;
	}

	glm::vec3 one;
	glm::vec3 two;
	glm::vec3 three;
private:

};