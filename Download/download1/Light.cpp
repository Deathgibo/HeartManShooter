#include "Light.h"

Light::Light()
{
	color = glm::vec3(0, 0, 0);
	intensity = 0.0f;
}

Light::Light(glm::vec3 col, float inten)
{
	color = col;
	intensity = inten;
}

void Light::UseLight(Shader* shader)
{
	glUseProgram(shader->program);

	glUniform3fv(shader->ambientcolor, 1, glm::value_ptr(color));
	glUniform1f(shader->ambientintensity, intensity);
}

Light::~Light()
{
}
