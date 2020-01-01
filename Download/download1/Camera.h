#pragma once

#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 pos, glm::vec3 worldup, float yawz, float pitchz);

	glm::mat4 CreateLookAtFunction();

	void handlekeyinput(bool* keys);
	void handlemouseinput(float xchange, float ychange);
	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetLookDirection() { return lookdirection; }

	void update();

	~Camera();

	glm::vec3 position;
	glm::vec3 lookdirection;
	glm::vec3 rightdirection;
	glm::vec3 updirection;
	glm::vec3 worldupdirection;
	float yaw;
	float pitch;
private:


	float movespeed;
	float turnspeed;
	float xrotation;
};


