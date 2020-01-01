#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0, 0, 0);
	cutoff = 0.0f;
	procEdge = 0.0f;
}

SpotLight::SpotLight(glm::vec3 dir, float cut, float proc, glm::vec3 pos, float con, float lin, float exp, glm::vec3 col, float inten) : PointLight(pos,con,lin,exp,col,inten)
{
	direction = dir;
	cutoff = cut;
	procEdge = proc;
}

void SpotLight::UseSpotLight(Shader* shader, int index)
{
	glUseProgram(shader->program);

	glUniform3fv(shader->uniformSpotLight[index].uniformColour, 1, glm::value_ptr(color));
	glUniform3fv(shader->uniformSpotLight[index].uniformPosition, 1, glm::value_ptr(position));
	glUniform3fv(shader->uniformSpotLight[index].uniformDirection, 1, glm::value_ptr(direction));
	glUniform1f(shader->uniformSpotLight[index].uniformAmbientIntensity, intensity);
	glUniform1f(shader->uniformSpotLight[index].uniformConstant, constant);
	glUniform1f(shader->uniformSpotLight[index].uniformLinear, linear);
	glUniform1f(shader->uniformSpotLight[index].uniformExponent, exponent);
	glUniform1f(shader->uniformSpotLight[index].uniformcutoff, cutoff);
	glUniform1f(shader->uniformSpotLight[index].uniformprocedge, procEdge);
}

void SpotLight::Calculateup()
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

SpotLight::~SpotLight()
{
}
