#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DirectionalLight.h"
#include "SpotLight.h"

class ShadowMap
{
public:
	ShadowMap();

	void CreateShadowMap(float w, float h, DirectionalLight light, float size);
	void CreateShadowMap(float w, float h, PointLight& light);
	void CreateShadowMap(float w, float h, SpotLight light);
	void CreateLightMatrix(DirectionalLight light);
	void CreateSpotLightMatrix(SpotLight spot);
	void CreatePointLightMatrix();
	void UseMap(Shader& shader);
	void UsePointMap(Shader& shader);
	~ShadowMap();

	GLuint fbo, depthmap, cubemap;
	float width, height;
	float Near, Far;
	glm::mat4 lightmatrix;
	glm::mat4 pointlightmatrix[6];
	float lightdistance;
	float orthosize;

	PointLight* pointref;
private:

};

