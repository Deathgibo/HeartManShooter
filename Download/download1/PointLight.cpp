#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0, 0, 0);
	constant = 0;
	linear = 0;
	exponent = 0;
}

PointLight::PointLight(glm::vec3 pos, float con, float lin, float exp, glm::vec3 col, float inten) : Light(col,inten)
{
	position = pos;
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::Initialize(glm::vec3 pos, float con, float lin, float exp, glm::vec3 col, float inten)
{
	position = pos;
	constant = con;
	linear = lin;
	exponent = exp;
	
	color = col;
	intensity = inten;
}

void PointLight::UsePointLight(Shader* shader, int index)
{
	glUseProgram(shader->program);

	glUniform3fv(shader->uniformpointlight[index].uniformColour, 1, glm::value_ptr(color));
	glUniform1f(shader->uniformpointlight[index].uniformAmbientIntensity, intensity);
	glUniform3fv(shader->uniformpointlight[index].uniformPosition, 1, glm::value_ptr(position));
	glUniform1f(shader->uniformpointlight[index].uniformconstant, constant);
	glUniform1f(shader->uniformpointlight[index].uniformlinear, linear);
	glUniform1f(shader->uniformpointlight[index].uniformexponent, exponent);
}

PointLight::~PointLight()
{

}
