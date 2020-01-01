#include "Material.h"

Material::Material()
{
	ambient = glm::vec3(1, 1, 1);
	diffuse = glm::vec3(1, 1, 1);
	specular = glm::vec3(1, 1, 1);
	shininess = 1.0f;
}

Material::Material(glm::vec3 amb, glm::vec3 dif, glm::vec3 spec, float shine)
{
	ambient = amb;
	diffuse = dif;
	specular = spec; 
	shininess = shine;
}

void Material::UseMaterial(Shader* shader)
{
	glUseProgram(shader->program);

	glUniform3fv(shader->ambient, 1, glm::value_ptr(ambient));
	glUniform3fv(shader->diffuse, 1, glm::value_ptr(diffuse));
	glUniform3fv(shader->specular, 1, glm::value_ptr(specular));
	glUniform1f(shader->shininess, shininess);
}

Material::~Material()
{

}
