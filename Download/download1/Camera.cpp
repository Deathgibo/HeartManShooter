#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0, 0, 0);
	lookdirection = glm::vec3(0, 0, 0);
}

Camera::Camera(glm::vec3 pos, glm::vec3 worldup, float yawz, float pitchz)
{
	position = pos;
	worldupdirection = worldup;

	yaw = yawz;
	pitch = pitchz;
	xrotation = 0;

	lookdirection = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);

	update();
}

glm::mat4 Camera::CreateLookAtFunction()
{
	return glm::lookAt(position, position + lookdirection, updirection);
}

void Camera::handlekeyinput(bool * keys)
{
	movespeed = 0.5f;

	if (keys[GLFW_KEY_W])
	{
		position += lookdirection * movespeed;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= lookdirection * movespeed;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= rightdirection * movespeed;
	}
	if (keys[GLFW_KEY_D])
	{
		position += rightdirection * movespeed;
	}
	if (keys[GLFW_KEY_LEFT_CONTROL])
	{
		position -= updirection * movespeed;
	}
	if (keys[GLFW_KEY_SPACE])
	{
		position += updirection * movespeed;
	}

	update();
}

void Camera::handlemouseinput(float xchange, float ychange)
{
	turnspeed = 0.05f;

	yaw += xchange * turnspeed;
	yaw = fmod(yaw, 360);


	pitch -= ychange * turnspeed;
	xrotation += xchange * turnspeed;


	/*
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	*/

	if (pitch > 179.0f)
	{
		pitch = 179.0f;
	}

	if (pitch < 1.0f)
	{
		pitch = 1.0f;
	}


	update();
}

void Camera::update()
{
	lookdirection.x = sin(glm::radians(pitch)) * cos(glm::radians(yaw));
	lookdirection.y = cos(glm::radians(pitch));
	lookdirection.z = sin(glm::radians(pitch)) * sin(glm::radians(yaw));
	lookdirection = glm::normalize(lookdirection);

	rightdirection = glm::normalize(glm::cross(lookdirection, worldupdirection));
	updirection = glm::normalize(glm::cross(rightdirection, lookdirection));
}

Camera::~Camera()
{

}
