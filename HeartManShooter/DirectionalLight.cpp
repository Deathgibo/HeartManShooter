#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0, 0, 0);
}

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 col, float inten) : Light(col,inten)
{
	direction = dir;
	Calculateup();
}

void DirectionalLight::UseDirLight(Shader* shader, int index)
{
	glUseProgram(shader->program);

	glUniform3fv(shader->uniformdirectionallight[index].uniformColour, 1, glm::value_ptr(color));
	glUniform1f(shader->uniformdirectionallight[index].uniformAmbientIntensity, intensity);
	glUniform3fv(shader->uniformdirectionallight[index].uniformDirection, 1, glm::value_ptr(direction));
}

void DirectionalLight::Calculateup()
{
	//find the phi and theta of look at and apply that to (0,1,0);
	glm::vec3 upvector(0.0f, 1.0f, 0.0f);
	glm::vec3 rightvector(1.0f, 0.0f, 0.0f);
	glm::vec3 normalvector = direction;
	normalvector = glm::normalize(normalvector);
	float ntheta = 1.57f;
	float nphi = 1.57f;
	float rtheta = 0.0f;
	float rphi = 1.57f;

	float phi, theta;
	if (normalvector.y > 1)
		normalvector.y = 1;
	if (normalvector.y < -1)
		normalvector.y = -1;
	assert(normalvector.y <= 1 && normalvector.y >= -1, "noramalvector.y needs to fit acos domain 1");

	phi = acos(normalvector.y);

	if (phi < .01)
		phi = .01;

	float calc = normalvector.x / sin(phi);
	if (calc > 1)
		calc = 1;
	if (calc < -1)
		calc = -1;
	assert(calc <= 1 && calc >= -1, "noramalvector.y needs to fit acos domain 2");

	theta = acos(calc);

	if (normalvector.z > 0)
	{
		theta = (3.14 * 2) - theta;
	}

	float thetadiff = theta - ntheta;

	rightvector.x = sin(rphi)*cos(rtheta + thetadiff);
	rightvector.y = cos(rphi);
	rightvector.z = sin(rphi)*sin(rtheta + thetadiff);
	rightvector = glm::normalize(rightvector);

	upvector = glm::cross(rightvector, normalvector);
	upvector = glm::normalize(upvector);

	if (upvector.y < 0)
	{
		upvector.y = -upvector.y;
	}
	up = upvector;
}

DirectionalLight::~DirectionalLight()
{

}
