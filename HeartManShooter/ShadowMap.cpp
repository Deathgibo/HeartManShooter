#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	width = 0;
	height = 0;
	lightmatrix = glm::mat4(1.0f);
	lightdistance = 550.0f;

	pointref = nullptr;
}

void ShadowMap::CreateShadowMap(float w, float h, PointLight& light)
{
	width = w;
	height = h;
	pointref = &light;

	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &depthmap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthmap);
	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthmap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %s\n", Status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Near = 1.0f;
	Far = 100.0f;
	CreatePointLightMatrix();
}

void ShadowMap::CreateShadowMap(float w, float h, DirectionalLight light, float size)
{
	width = w;
	height = h;

	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &depthmap);
	glBindTexture(GL_TEXTURE_2D, depthmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %s\n", Status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	orthosize = size;
	CreateLightMatrix(light);
}

void ShadowMap::CreateShadowMap(float w, float h, SpotLight light)
{
	width = w;
	height = h;

	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &depthmap);
	glBindTexture(GL_TEXTURE_2D, depthmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %s\n", Status);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CreateSpotLightMatrix(light);
}

void ShadowMap::CreateLightMatrix(DirectionalLight light)
{
	light.Calculateup();

	float near = 1.0f; 	float far = 1000.5f;

	glm::mat4 ortho = glm::ortho(-orthosize, orthosize, -orthosize, orthosize, near, far);
	glm::mat4 view = glm::lookAt(-glm::normalize(light.direction) * lightdistance, glm::vec3(0, 0, 0), light.up);//-glm::normalize(light.direction) * lightdistance
	
	
	//cout << light.up.x << " " << light.up.y << " " << light.up.z << endl;
	lightmatrix = ortho * view * glm::mat4(1.0f);
}

void ShadowMap::CreateSpotLightMatrix(SpotLight spot)
{
	spot.Calculateup();

	float near = 0.1f; 	float far = 1000.5f;

	glm::mat4 proj = glm::perspective(1.57f, (width / height), near, far);//1.0472f
	glm::mat4 view = glm::lookAt(spot.position, spot.position + spot.direction, spot.up);

	lightmatrix = proj * view * glm::mat4(1.0f);
}

void ShadowMap::CreatePointLightMatrix()
{

	glm::mat4 proj = glm::perspective(Utils::ToRadians(90.0f), (width / height), Near, Far);

	glm::mat4 viewl = glm::lookAt(pointref->position, pointref->position + glm::vec3(-1.0f, 0, 0), glm::vec3(0, -1.0f, 0));
	glm::mat4 viewr = glm::lookAt(pointref->position, pointref->position + glm::vec3(1.0f, 0, 0), glm::vec3(0, -1.0f, 0));
	glm::mat4 viewt = glm::lookAt(pointref->position, pointref->position + glm::vec3(0, 1.0f, 0), glm::vec3(0, 0, 1.0f));
	glm::mat4 viewb = glm::lookAt(pointref->position, pointref->position + glm::vec3(0, -1.0f, 0), glm::vec3(0, 0, -1.0f));
	glm::mat4 viewf = glm::lookAt(pointref->position, pointref->position + glm::vec3(0, 0, -1.0f), glm::vec3(0, -1.0f, 0));
	glm::mat4 viewbk = glm::lookAt(pointref->position, pointref->position + glm::vec3(0, 0, 1.0f), glm::vec3(0, -1.0f, 0));

	pointlightmatrix[0] = proj * viewr;
	pointlightmatrix[1] = proj * viewl;
	pointlightmatrix[2] = proj * viewt;
	pointlightmatrix[3] = proj * viewb;
	pointlightmatrix[4] = proj * viewbk;
	pointlightmatrix[5] = proj * viewf;
}

void ShadowMap::UseMap(Shader& shader)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glUseProgram(shader.program);
	glUniformMatrix4fv(shader.lightmatrix, 1, GL_FALSE, glm::value_ptr(lightmatrix));
	glViewport(0, 0, width, height);
	glCullFace(GL_FRONT);
}

void ShadowMap::UsePointMap(Shader& shader)
{
	CreatePointLightMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glUseProgram(shader.program);
	glUniform4fv(shader.lightPos, 1, glm::value_ptr(pointref->position));
	glUniform1f(shader.far_plane, Far);
	for (int i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(shader.pointMatrix[i], 1, GL_FALSE, glm::value_ptr(pointlightmatrix[i]));
	}
	glViewport(0, 0, width, height);
	glCullFace(GL_FRONT);
}

ShadowMap::~ShadowMap()
{
	//delete framebuffer
	//delete textures
}
