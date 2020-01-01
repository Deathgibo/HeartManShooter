#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <fstream>

#include <SDL_mixer.h>

#include <GL\glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Sphere.h"
#include "Rectangle.h"
#include "Camera.h"
#include "Texture.h"
#include "Texture3D.h"
#include "Model.h"
#include "Player.h"
#include "Weapon.h"
#include "HitBox.h"	
#include "Plane.h"
#include "HitPlane.h"
#include "Triangle.h"
#include "Square.h"
#include "Ray.h"
#include "ParticleSpawner_Fire.h"
#include "ParticleSpawn_Fog.h"
#include "ParticleSpawner_Water.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "ShadowMap.h"
#include "Torch.h"
#include "Water.h"
#include "CubeTexture.h"
#include <time.h>
#include <chrono>
using namespace std::chrono;
using namespace std;

glm::vec3 MarioOffset(3.5f,2.8f,3.5f);
glm::vec3 NanSuitOffset(1.0f, 14.2f, 1.0f);

Mix_Music* sound_akfiring = 0;
Mix_Chunk* sound_akreload = 0;
Mix_Chunk* sound_aklastbullet = 0;
Mix_Chunk* sound_pistolshot = 0;
Mix_Chunk* sound_pistolreload = 0;
Mix_Chunk* sound_awpshot = 0;
Mix_Chunk* sound_awpreload = 0;

/*Modes*/
bool thirdpersonmode = false;
bool editormode = false;
bool noclip = false;
bool camerafreelock = false;
int LEVEL = 0;

/*Hitbox editor stuff*/
bool hitboxon = true;
int hitboxindex = 0;
int hitplaneindex = 0;

/*GLFW Stuff*/
GLFWwindow* window;

int RESOLUTION_X = 1280;
int RESOLUTION_Y = 720;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;
float thexchange;
float theychange;

double MS_PER_UPDATE = 10.0;//10.0

Shader shaderinstance;
Shader shadermain;
Shader shader2d;
Shader shaderparticle;
Shader shadertess;
Shader shadernormals;
Shader shadershadow;
Shader shadersimple;
Shader shaderomni;
Shader shaderomni2;
Shader shaderwater;
Shader shaderskybox;

Model CrashB;
Model Tree;
Model Helicopter;
Model Nanosuit;
Model AwpMap;
Model Mario;
Model WorldMap;
Model Bullet;
Model PistolBullet;
Model Flash;
Model Dino;
Model World;
Model theTorch;

vector<Triangle> WorldTriangles;

Weapon Ak47;
Weapon Awp;
Weapon Pistol;

Player player1;

vector<HitBox> staticHitBoxes;
vector<HitPlane> staticHitPlanes;

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 270.0f, 90.0f);
Camera camera3Person(glm::vec3(0.0f, 5.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 270.0f, 100.0f);
Camera cameraFree(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 270.0f, 90.0f);
Camera* currentcamera = nullptr;

Texture checkerboard("Textures/glass.png");
Texture redtexture("Textures/red.png");
Texture darkglass("Textures/darkglass.png");
Texture missingtex("Textures/missingtexture.png");
Texture3D striped3d;
Texture flashtex("Textures/gunflash.png");
Texture bullethole("Textures/bullethole3.png");
Texture sniperscope("Textures/scope2.jpg");
Texture fogtexture("Textures/fog2.jpg");
Texture firetexture("Textures/fire.png");
Texture dvdMap("Textures/waterDUDV.png");
Texture waternormalmap("Textures/normal.png");
CubeTexture skybox1;

Rectangle flashrect;
Square scoperect;
Square depthmaprect;
Square glassrect1;
Square glassrect2;
Square glassrect3;
Rectangle skyboxrect;


ParticleSpawner_Fire Fire_Spawner;
ParticleSpawn_Fog Fog_Spawner;
ParticleSpawner_Water fountain_spawner1;
ParticleSpawner_Water fountain_spawner2;

GLuint dummyvao;
GLuint dummyvbo;

/*Lights*/
Light ambient1(glm::vec3(1.0f, 1.0f, 1.0f), 0.25f);
DirectionalLight diffuse1(glm::vec3(0.0f, -0.8f, 0.2f), glm::vec3(1.0f, 0.7f, 0.7f), .8f);
DirectionalLight diffuse2(glm::vec3(0.2f, -0.3f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), .6f);


PointLight point1(glm::vec3(360, 10, 20), 1.0f, .0005f, .00009f, glm::vec3(1.0f, 0.3f, 0.0f), 1.0f);
Torch torch1;
Torch torch2;
Torch torch3; 
Torch torch4;
Torch torch5;
Torch torch6;

glm::vec3 point1position = glm::vec3(15.0f, 70.0f, 0.0f);
SpotLight spot1(glm::vec3(0.0, -1.0f, 0.0f), cos(Utils::ToRadians(60)), cos(Utils::ToRadians(4)),
	glm::vec3(600, 200.0f, 0.0f), 1.0f, .0f, .0f, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
SpotLight spot2(glm::vec3(1.0f, 0.0f, 0.0f), cos(Utils::ToRadians(45)), cos(Utils::ToRadians(4)),
	glm::vec3(50.0f, 10.0f, 0.0f), 1.0f, .0f, .0f, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);


/*Shadow Maps*/
ShadowMap shadow_diffuse1;
ShadowMap shadow_diffuse2;
ShadowMap shadow_spot1;
ShadowMap shadow_spot2;
ShadowMap shadow_point1;
ShadowMap shadow_point2;
vector<ShadowMap*> shadow_Diffuse;
vector<ShadowMap*> shadow_Spot;
vector<ShadowMap*> shadow_Point;

/*Water*/
Square reflectbox;
Square refractbox;
Water water1;

/*Materials*/
Material mat_nospecular(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
Material mat_specularsmall(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 256.0f);
Material mat_specularmedium(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 100.0f);
Material mat_gold(glm::vec3(.24725f, .1995f, .0745f), glm::vec3(.75164f, .60648, .22648f), glm::vec3(.628281f, .555802f, .366065f), .4f * 128);
Material mat_jade(glm::vec3(.135f, .2225f, .1575f), glm::vec3(.54f, .89f, .63f), glm::vec3(.3162f, .3162f, .3162f), 12.8);


GLuint mainfbo, maincolormap, maindepthmap;
Square mainscreen;

/*input stuff*/
bool globalkeys[1024];
Singlekeys singlekeys[1024];
bool mouseFirstMoved = true;
float mousex, mousey;
bool rightclick = false;
bool leftclick = false;

void ChangeLevels(int level);
void Input();
void HandleButtons();
void Update();
void Render(Shader& shader, bool shadowpass, bool waterpass);
void MouseHandler(float& x, float& y);
void CreateStaticHitboxes();
void CreateStaticPlanes();
void HandleEditorInput();
void close();
int InitializeGLFWandGLEW();
bool InitializeSDL();
void SetupMeshes();
void SetPerspectiveMatrix();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
int main(int argc, char* argv[])
{
	/*Initialize*/
	if (InitializeGLFWandGLEW() == -1)
	{
		cout << "Error initializing GLFW and GLEW!" << endl;
		return -1;
	}
	if (!InitializeSDL())
	{
		cout << "Could not initialize SDL!" << endl;
		return -1;
	}
	Utils::SetRandomSeed();

	/*Meshes & Hitboxes*/
	SetupMeshes();
	CreateStaticHitboxes();
	CreateStaticPlanes();
	flashrect.GenerateFlatRectangle(.7f, .7f);
	scoperect.createsquare(1, 1);
	depthmaprect.createsquare(.3f, .3f);

	glassrect1.createsquareflat(200, 200);
	glassrect2.createsquare(300, 200);
	glassrect3.createsquareflat(200, 200);

	/*Water*/
	reflectbox.createsquare(.3f, .3f);
	refractbox.createsquare(.3f, .3f);
	water1.CreateWater(720, 680);//1040 680
	water1.CreateWaterMesh(110, 78);
	water1.meshheight = 3.5f;

	/*Player*/
	player1.Initialize(&Mario, &Ak47);

	/*Shaders*/
	shadermain.createShaderProgram("Shaders/shadermain/vertexshader.glsl.txt", "Shaders/shadermain/fragmentshader.glsl.txt");
	shader2d.createShaderProgram("Shaders/shader2d/vertexshader2d.glsl.txt", "Shaders/shader2d/fragmentshader2d.glsl.txt");
	shaderparticle.createShaderProgram("Shaders/shaderparticle/vertexshaderparticle.glsl.txt", "Shaders/shaderparticle/fragmentshaderparticle.glsl.txt");
	shadertess.createShaderProgram("Shaders/shadertess/tesselatorvertex.glsl.txt", "Shaders/shadertess/tesselatorfrag.glsl.txt", "Shaders/shadertess/tesselatorcontrol.glsl.txt", "Shaders/shadertess/tesselatorevaluator.glsl.txt");
	shadernormals.createShaderProgram("Shaders/shadernormals/vertexshadernormal.glsl.txt", "Shaders/shadernormals/fragmentshadernormal.glsl.txt", "Shaders/shadernormals/geometryshadernormal.glsl.txt");
	shadershadow.createShaderProgram("Shaders/shadershadowmap/shadowvertex.glsl.txt", "Shaders/shadershadowmap/shadowfrag.glsl.txt");
	shadersimple.createShaderProgram("Shaders/shadersimple/simplervertex.glsl.txt", "Shaders/shadersimple/simplefrag.glsl.txt");
	shaderomni.createShaderProgram("Shaders/shaderomnishadow/omnivert.txt", "Shaders/shaderomnishadow/omnifrag.txt", "Shaders/shaderomnishadow/omnigeom.txt");
	shaderomni2.createShaderProgram("Shaders/shaderomni2/omnivert.txt", "Shaders/shaderomni2/omnifrag.txt");
	shaderwater.createShaderProgram("Shaders/shaderwater/vertexwater.glsl.txt", "Shaders/shaderwater/fragmentwater.glsl.txt");
	shaderskybox.createShaderProgram("Shaders/shaderskybox/skyboxvertex.txt", "Shaders/shaderskybox/skyboxfragment.txt");

	/*Textures*/
	checkerboard.LoadTextureA();
	redtexture.LoadTextureA();
	darkglass.LoadTextureA();
	missingtex.LoadTextureA();
	flashtex.LoadTextureA();
	bullethole.LoadTextureA();
	sniperscope.LoadTextureA();
	fogtexture.LoadTextureA();
	firetexture.LoadTextureA();
	dvdMap.LoadTextureA();
	waternormalmap.LoadTextureA();

	striped3d.generate3dpattern();
	striped3d.load3dtexture();
	
	/*sky box*/
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/valley-of-the-dead_rt.tga");
	skyboxFaces.push_back("Textures/valley-of-the-dead_lf.tga");
	skyboxFaces.push_back("Textures/valley-of-the-dead_up.tga");
	skyboxFaces.push_back("Textures/valley-of-the-dead_dn.tga");
	skyboxFaces.push_back("Textures/valley-of-the-dead_bk.tga");
	skyboxFaces.push_back("Textures/valley-of-the-dead_ft.tga");
	skybox1.LoadTexture(skyboxFaces);
	skyboxrect.GenerateRectangle(1, 1, 1);

	/*Camera*/
	SetPerspectiveMatrix();
	currentcamera = &camera;

	/*Particles*/
	Fire_Spawner.Initialize(5.8f, 5.8f, 5000);
	Fire_Spawner.delay = 1;
	Fire_Spawner.particlelife = 120;
	Fire_Spawner.particlevelocity = .3f;
	Fire_Spawner.spawnspeed = 12;
	Fire_Spawner.xarea = 500;
	Fire_Spawner.zarea = 1500;
	Fire_Spawner.particlecenter = glm::vec3(380, 5, 77);
	Fog_Spawner.Initialize(5.8f, 5.8f, 5000);
	fountain_spawner1.Initialize(5.8f, 5.8f, 1000);
	fountain_spawner1.spawncenter = glm::vec3(17, 5, 0);
	fountain_spawner2.Initialize(5.8f, 5.8f, 1000);
	fountain_spawner2.spawncenter = glm::vec3(-112, 5, 0);


	/*lighting*/
	torch1.Initialize(glm::vec3(-55.0f, 35.0f, -75.0f), 1.0f, .003f, .001f, glm::vec3(1.0f, 0.8f, 0.8f), 1.0f, theTorch,0,-5.0f,0);
	torch1.SetParticleSpawner(glm::vec3(-55.0f + 0.9f, 35.0f - 1.0f, -75.0f + 1.0f), 60, .03f, 1, 20.0f, 20.0f,1.0f,1);
	
	torch2.Initialize(glm::vec3(-55.0f, 35.0f, 75.0f), 1.0f, .003f, .001f, glm::vec3(1.0f, 0.8f, 0.8f), 1.0f, theTorch,0, -5.0f, 0);
	torch2.SetParticleSpawner(glm::vec3(-55.0f + 0.9f, 35.0f - 1.0f, 75.0f + 1.0f), 60, .03f, 1, 20.0f, 20.0f, 1.0f, 1);
	
	torch3.Initialize(glm::vec3(-55.0f, 35.0f, 272.0f), 1.0f, .003f, .001f, glm::vec3(1.0f, 0.8f, 0.8f), 1.0f, theTorch, 0, -5.0f, 0);
	torch3.SetParticleSpawner(glm::vec3(-55.0f + 0.9f, 35.0f - 1.0f, 272.0f + 1.0f), 60, .03f, 1, 20.0f, 20.0f, 1.0f, 1);
	
	torch4.Initialize(glm::vec3(-387.0f, 35.0f, 0.0f), 1.0f, .003f, .001f, glm::vec3(1.0f, 0.8f, 0.8f), 1.0f, theTorch, 0, -5.0f, 0);
	torch4.SetParticleSpawner(glm::vec3(-387.0f + 0.9f, 35.0f - 1.0f, 0.0f + 1.0f), 60, .03f, 1, 20.0f, 20.0f, 1.0f, 1);
	
	torch5.Initialize(glm::vec3(230.0f, 170.0f, 270.0f), 1.0f, .0003f, .00009f, glm::vec3(1.0f, 0.8f, 0.8f), 1.0f, theTorch, 0, -13.0f, 0);
	torch5.SetParticleSpawner(glm::vec3(230.0f + 4.0f, 170.0f + 3.0f, 270.0f + 4.0f), 60, .1f, 1, 80.0f, 80.0f, 3.0f, 1);
	
	torch6.Initialize(glm::vec3(230.0f, 170.0f, -270.0f), 1.0f, .0003f, .00009f, glm::vec3(1.0f, 0.8f, 0.8f), 1.0f, theTorch, 0, -13.0f, 0);
	torch6.SetParticleSpawner(glm::vec3(230.0f + 4.0f, 170.0f + 3.0f, -270.0f + 4.0f), 60, .1f, 1, 80.0f, 80.0f, 3.0f, 1);

	/*Shadow Maps*/
	shadow_diffuse1.CreateShadowMap(3000, 3000, diffuse1,600);
	shadow_diffuse2.CreateShadowMap(4096, 4096, diffuse2,200);
	shadow_Diffuse.push_back(&shadow_diffuse1);
	//shadow_Diffuse.push_back(&shadow_diffuse2);

	shadow_spot1.CreateShadowMap(2048, 2048, spot1);
	shadow_spot2.CreateShadowMap(2048, 2048, spot2);
	shadow_Spot.push_back(&shadow_spot1);
	//shadow_Spot.push_back(&shadow_spot2);

	//shadow_point1.CreateShadowMap(1024, 1024, point1);
	//shadow_point2.CreateShadowMap(1024, 1024, point2);
	//shadow_Point.push_back(&shadow_point1);
	//shadow_Point.push_back(&shadow_point2);

	/*MAIN FBO*/
	mainscreen.createsquare(1, 1);

	glGenFramebuffers(1, &mainfbo);

	glGenTextures(1, &maincolormap);
	glBindTexture(GL_TEXTURE_2D, maincolormap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RESOLUTION_X, RESOLUTION_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &maindepthmap);
	glBindTexture(GL_TEXTURE_2D, maindepthmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, RESOLUTION_X, RESOLUTION_Y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, mainfbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, maincolormap, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, maindepthmap, 0);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %s\n", Status);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*Extras*/
	auto previous = high_resolution_clock::now();
	double lag = 0.0;

	glGenVertexArrays(1, &dummyvao);
	glGenBuffers(1, &dummyvbo);

	float fps = 0;
	for (int i = 0; i < 1024; i++)
	{
		singlekeys[i].key = false;
	}
	Utils::checkOpenGLError();

	ChangeLevels(0);
	/*					MAIN LOOP					*/
	while (!glfwWindowShouldClose(window))
	{
		auto fps_start = high_resolution_clock::now();
			
		auto current = high_resolution_clock::now();
		auto elasped = duration_cast<milliseconds>(current - previous);
		previous = current;
		lag += elasped.count();
		Input();	

		while (lag >= MS_PER_UPDATE)
		{
			Update();
			lag -= MS_PER_UPDATE;
			for (int i = 0; i < 1024; i++)
			{
				singlekeys[i].key = false;
			}
		}

		//Fill all our depth buffers
		/*Diffuse Shadow Pass*/
		for (int i = 0; i < shadow_Diffuse.size(); i++)
		{
			shadow_Diffuse[i]->UseMap(shadershadow);
			Render(shadershadow, true,false);
		}

		/*Spot Shadow Pass*/
		for (int i = 0; i < shadow_Spot.size(); i++)
		{
			shadow_Spot[i]->UseMap(shadershadow);
			Render(shadershadow, true, false);
		}

		/*Point Shadow Pass*/
		for (int j = 0; j < shadow_Point.size(); j++)
		{
			for (int i = 0; i < 6; i++)
			{
				shadow_Point[j]->UsePointMap(shaderomni2);
				GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, face, shadow_Point[j]->depthmap, 0);
				glUniform1i(shaderomni2.isflash, i);
				Render(shaderomni2, true, false);
			}
		}



		/*Regular Pass*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
		glViewport(0, 0, width, height);
		glUseProgram(shadermain.program);

		/*Diffuse Uniforms*/
		for (int i = 0; i < shadow_Diffuse.size(); i++)//3-8
		{
			glActiveTexture(GL_TEXTURE3 + i);
			glBindTexture(GL_TEXTURE_2D, shadow_Diffuse[i]->depthmap);
			glUniform1i(shadermain.diffusedepthmaps[i], 3 + i);

			glUniformMatrix4fv(shadermain.diffusematrix[i], 1, GL_FALSE, glm::value_ptr(shadow_Diffuse[i]->lightmatrix));
		}

		/*Spot Uniforms*/
		for (int i = 0; i < shadow_Spot.size(); i++)//9-14
		{
			glActiveTexture(GL_TEXTURE9 + i);
			glBindTexture(GL_TEXTURE_2D, shadow_Spot[i]->depthmap);
			glUniform1i(shadermain.spotdepthmaps[i], 9 + i);

			glUniformMatrix4fv(shadermain.spotmatrix[i], 1, GL_FALSE, glm::value_ptr(shadow_Spot[i]->lightmatrix));
		}

		/*Point Uniforms*/
		for (int i = 0; i < shadow_Point.size(); i++)//15-20
		{
			glActiveTexture(GL_TEXTURE15 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_Point[i]->depthmap);
			glUniform1i(shadermain.pointdepthmaps[i], 15 + i);

			glUniform4fv(shadermain.pointlightpos[i], 1, glm::value_ptr(shadow_Point[i]->pointref->position));
			glUniform1f(shadermain.pointfar_planes[i], shadow_Point[i]->Far);
		}

		if (LEVEL == 0)
		{
			/*water reflect*/
			glBindFramebuffer(GL_FRAMEBUFFER, water1.fboreflect);
			glEnable(GL_CLIP_DISTANCE0);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glCullFace(GL_BACK);
			glViewport(0, 0, water1.width, water1.height);
			glUseProgram(shadermain.program);
			glUniform1i(shadermain.clipping, 1);
			glUniform4fv(shadermain.clip_plane, 1, glm::value_ptr(glm::vec4(0, 1.0f, 0, water1.meshheight)));
			/*camera change*/
			float distance = 2 * (currentcamera->position.y - water1.meshheight);
			currentcamera->position.y -= distance;
			currentcamera->pitch = currentcamera->pitch + (2 * (90 - currentcamera->pitch));
			currentcamera->update();

			vMat = currentcamera->CreateLookAtFunction();

			currentcamera->position.y += distance;
			currentcamera->pitch = currentcamera->pitch + (2 * (90 - currentcamera->pitch));
			currentcamera->update();

			Render(shadermain, true, true);

			/*water refract*/
			glBindFramebuffer(GL_FRAMEBUFFER, water1.fborefract);
			glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glViewport(0, 0, water1.width, water1.height);
			glUseProgram(shadermain.program);
			glUniform1i(shadermain.clipping, 1);
			glUniform4fv(shadermain.clip_plane, 1, glm::value_ptr(glm::vec4(0, -1.0f, 0, -water1.meshheight)));
			vMat = currentcamera->CreateLookAtFunction();
			Render(shadermain, true, true);
		}

		/*main fbo render*/
		glBindFramebuffer(GL_FRAMEBUFFER, mainfbo);
		glDisable(GL_CLIP_DISTANCE0);
		glCullFace(GL_BACK);
		glViewport(0, 0, RESOLUTION_X, RESOLUTION_Y);
		glUseProgram(shadermain.program);
		glUniform1i(shadermain.clipping, 0);
		Render(shadermain, false, false);

		/*render quad*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glUseProgram(shader2d.program);
		glUniform1i(shader2d.renderdepthmap, 1);
		glUniform1i(shader2d.depthmap, 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, maincolormap);
		mMat = glm::mat4(1.0f);
		mvMat = mMat;
		glUniformMatrix4fv(shader2d.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		mainscreen.RenderMesh();
		glUseProgram(shadermain.program);

		/*main render*/
		/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CLIP_DISTANCE0);
		glCullFace(GL_BACK);
		glViewport(0, 0, width, height);
		glUseProgram(shadermain.program);
		glUniform1i(shadermain.clipping, 0);
		Render(shadermain, false, false);*/
		
		//cout<< glGetError();



		/* ADD 60 CAP OPTION*/


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		
		auto fps_stop = high_resolution_clock::now();
		auto fps_duration = duration_cast<microseconds>(fps_stop - fps_start);
		fps = 1 / (fps_duration.count() / 1000000.0f);
		int fpsint = fps;
		string fpsstring = "FPS: ";
		fpsstring.append(to_string(fpsint));
		const char* lol = fpsstring.c_str();
		glfwSetWindowTitle(window, lol);

		if (globalkeys[GLFW_KEY_ESCAPE])
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	close();
	glfwTerminate();
	return 0;
}

void Input()
{
	//MouseHandler(thexchange, theychange);
	//glfwWaitEvents();
	glfwPollEvents();

	/*camera handling*/
	//camera.handlemouseinput(thexchange, theychange);//*make sure camera mouse is b4 player1 handle and camera position because everything in this frame needs to use same camera directions
	//camera3Person.handlekeyinput(globalkeys);
	//camera3Person.handlemouseinput(thexchange, 0);

	if (!camerafreelock)
	{
		cameraFree.handlekeyinput(globalkeys);
		cameraFree.handlemouseinput(thexchange, theychange);
	}
	HandleButtons();//one time inputs
}

void Update()
{
	MouseHandler(thexchange, theychange);

	camera.handlemouseinput(thexchange, theychange);//*make sure camera mouse is b4 player1 handle and camera position because everything in this frame needs to use same camera directions

	player1.PlayerUpdate(singlekeys);
	player1.HandleInput(globalkeys);//*creating hitplane class cleanup


	camera3Person.handlekeyinput(globalkeys);
	camera3Person.handlemouseinput(thexchange, 0);
	camera.position = player1.position + glm::vec3(0, MarioOffset.y, 0) + MarioOffset.x*glm::normalize(glm::vec3(camera.lookdirection.x, 0, camera.lookdirection.z));
	camera3Person.position = camera.position + (8.0f*camera.worldupdirection) + (-39.0f*glm::normalize(glm::vec3(camera.lookdirection.x, 0, camera.lookdirection.z)));

	if (LEVEL == 0)
	{
		Fire_Spawner.Update();
		torch1.particlespawner.Update();
		torch2.particlespawner.Update();
		torch3.particlespawner.Update();
		torch4.particlespawner.Update();
		torch5.particlespawner.Update();
		torch6.particlespawner.Update();
		Fog_Spawner.Update();
		fountain_spawner1.Update();
		fountain_spawner2.Update();

		water1.Update();
	}	

}

void Render(Shader& shader, bool shadowpass, bool waterpass)
{
	/*FBO  - used to capture screen and can do post-proccessing effects, even get screen from different angle for reflection mapping
	GLuint fbo;
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		//good
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glDeleteFramebuffers(1,&fbo);
	
	//create texture
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

	//renderbuffer quick buffer for storing textures write-only
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, texture, rbo);
	*/

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glUseProgram(shader.program);
	glUniform1i(shader.textsample, 1);

	glUniformMatrix4fv(shader.projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniform3fv(shader.eyeposition, 1, glm::value_ptr(camera.position));
	if (!waterpass)
	{
		vMat = currentcamera->CreateLookAtFunction();
	}

	/*Set lights*/
	if (!shadowpass)//*for loop
	{
		ambient1.UseLight(&shader);

		diffuse1.UseDirLight(&shader, 0);
		torch1.pointlight.UsePointLight(&shader, 0);
		torch2.pointlight.UsePointLight(&shader, 1);
		torch3.pointlight.UsePointLight(&shader, 2);
		torch4.pointlight.UsePointLight(&shader, 3);
		torch5.pointlight.UsePointLight(&shader, 4);
		torch6.pointlight.UsePointLight(&shader, 5);
		point1.UsePointLight(&shader, 6);
		//diffuse2.UseDirLight(&shader, 1);
		//point1.position = point1position;
		//point1.UsePointLight(&shader, 0);
		//point2.UsePointLight(&shader, 1);
		spot1.UseSpotLight(&shader, 0);
		//spot2.UseSpotLight(&shader, 1);


		mat_nospecular.UseMaterial(&shader);
	}


	/*World*/
	glUseProgram(shader.program);
	mMat = glm::mat4(1.0f);
	//mMat = glm::scale(mMat, glm::vec3(15.0f, 15.0f, 15.0f));
	mvMat = vMat * mMat;
	glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	//WorldMap.RenderModel();
	World.RenderModel();

	if (LEVEL == 0)
	{
		/*Dino*/
		glUseProgram(shader.program);
		mMat = glm::mat4(1.0f);
		mMat = glm::translate(mMat, glm::vec3(250.0f, 5.0f, 120.0f));
		mMat = glm::scale(mMat, glm::vec3(15.0f, 15.0f, 15.0f));
		mvMat = vMat * mMat;
		glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
		glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		Dino.RenderModel();

		/*Dino 2*/
		glUseProgram(shader.program);
		mMat = glm::mat4(1.0f);
		mMat = glm::translate(mMat, glm::vec3(250.0f, 5.0f, -120.0f));
		mMat = glm::scale(mMat, glm::vec3(15.0f, 15.0f, 15.0f));
		mvMat = vMat * mMat;
		glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
		glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		Dino.RenderModel();
	}
	
	if (LEVEL == 0)
	{
		/*Torches*/
		torch1.Render(&shader);
		torch2.Render(&shader);
		torch3.Render(&shader);
		torch4.Render(&shader);
		torch5.RenderScale(&shader, 5, 5, 5);
		torch6.RenderScale(&shader, 5, 5, 5);
	}


	if (!shadowpass)
	{
		glUseProgram(shadernormals.program);
		mMat = glm::mat4(1.0f);
		mvMat = vMat * mMat;
		glUniformMatrix4fv(shadernormals.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
		glUniformMatrix4fv(shadernormals.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(shadernormals.projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		//WorldMap.RenderModel();
		//World.RenderModel();
	}

	/*Tesselator*/
	if (!shadowpass)
	{
		/*glUseProgram(shadertess.program);
		glBindTexture(GL_TEXTURE_2D, 0);
		mMat = glm::mat4(1.0f);
		mMat = glm::scale(mMat, glm::vec3(20, 20, 20));
		mvMat = vMat * mMat;
		glUniformMatrix4fv(shadertess.projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(shadertess.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
		glUniformMatrix4fv(shadertess.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

		glPatchParameteri(GL_PATCH_VERTICES, 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(dummyvao);
		glDrawArrays(GL_PATCHES, 0, 1);
		glBindVertexArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
	}
	
	/*Player (Weapon, Model, Bullets, Bulletholes)*/
	glUseProgram(shader.program);
	player1.RenderPlayer(&shader, shadowpass);

	if (LEVEL == 0)
	{
		/*Water*/
		if (!shadowpass)
		{
			glUseProgram(shadermain.program);
			mMat = glm::mat4(1.0f);
			mMat = glm::translate(mMat, glm::vec3(-50.0f, water1.meshheight, 2));
			mvMat = vMat * mMat;
			glUniformMatrix4fv(shadermain.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
			glUniformMatrix4fv(shadermain.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
			glUniformMatrix4fv(shadermain.projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
			glUniform3fv(shadermain.cameraposition, 1, glm::value_ptr(currentcamera->position));
			glUniform1f(shadermain.movefactor, water1.movefactor);
			glUniform1i(shadermain.renderingwater, 1);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, dvdMap.textureid);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, water1.colormapreflect);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, water1.colormaprefract);
			glActiveTexture(GL_TEXTURE22);
			glBindTexture(GL_TEXTURE_2D, waternormalmap.textureid);

			water1.mesh.RenderMesh();

			glUniform1i(shadermain.renderingwater, 0);
			glUseProgram(shader.program);
		}
	}
	/*depthmap helpers*/
	if (!shadowpass)
	{
		/*glUseProgram(shader2d.program);
		glUniform1i(shader2d.renderdepthmap, 1);
		glUniform1i(shader2d.depthmap, 3);
		glActiveTexture(GL_TEXTURE3);

		glBindTexture(GL_TEXTURE_2D, water1.colormapreflect);
		mMat = glm::mat4(1.0f);
		mMat = glm::translate(mMat, glm::vec3(-.7f, .7f, 0));
		mvMat = mMat;
		glUniformMatrix4fv(shader2d.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		reflectbox.RenderMesh();

		glBindTexture(GL_TEXTURE_2D, water1.colormaprefract);
		mMat = glm::mat4(1.0f);
		mMat = glm::translate(mMat, glm::vec3(.7f, .7f, 0));
		mvMat = mMat;
		glUniformMatrix4fv(shader2d.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		refractbox.RenderMesh();

		glUseProgram(shader.program);*/
	}

	if (!shadowpass)
	{
		glUseProgram(shadernormals.program);
		//player1.RenderPlayer(&shadernormals,shadowpass);
	}

	/*sky box*/
	if (!shadowpass || waterpass)
	{
		//glDepthMask(GL_FALSE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glUseProgram(shaderskybox.program);
		mvMat = vMat;
		glUniformMatrix4fv(shaderskybox.projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(shaderskybox.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		skybox1.UseTexture(0);
		skyboxrect.RenderMesh();
		//glDepthMask(GL_TRUE);
	}

	/*Particles*/
	if (!shadowpass)
	{
		if (LEVEL == 0)
		{
			Fire_Spawner.Render(&shaderparticle);
			//Fog_Spawner.Render(&shaderparticle);
			fountain_spawner1.Render(&shaderparticle);
			fountain_spawner2.Render(&shaderparticle);
			torch1.particlespawner.Render(&shaderparticle);
			torch2.particlespawner.Render(&shaderparticle);
			torch3.particlespawner.Render(&shaderparticle);
			torch4.particlespawner.Render(&shaderparticle);
			torch5.particlespawner.Render(&shaderparticle);
			torch6.particlespawner.Render(&shaderparticle);
		}
	}
	if (LEVEL == 0)
	{
		if (Utils::Distance3vec(player1.position, Fire_Spawner.particlecenter) > 200.0f && shadowpass)
		{
			Fire_Spawner.Render(&shaderparticle);
		}
		if (Utils::Distance3vec(player1.position, torch1.pointlight.position) > 30.0f && shadowpass)
		{
			torch1.particlespawner.Render(&shaderparticle);
		}
		if (Utils::Distance3vec(player1.position, torch2.pointlight.position) > 30.0f && shadowpass)
		{
			torch2.particlespawner.Render(&shaderparticle);
		}
		if (Utils::Distance3vec(player1.position, torch3.pointlight.position) > 30.0f && shadowpass)
		{
			torch3.particlespawner.Render(&shaderparticle);
		}
		if (Utils::Distance3vec(player1.position, torch4.pointlight.position) > 30.0f && shadowpass)
		{
			torch4.particlespawner.Render(&shaderparticle);
		}
		if (Utils::Distance3vec(player1.position, torch5.pointlight.position) > 30.0f && shadowpass)
		{
			torch5.particlespawner.Render(&shaderparticle);
		}
		if (Utils::Distance3vec(player1.position, torch6.pointlight.position) > 30.0f && shadowpass)
		{
			torch6.particlespawner.Render(&shaderparticle);
		}
	}


	/*glass*/
	if (!shadowpass || waterpass)
	{
		glUseProgram(shader.program);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1i(shader.isglass, 1);
		glUniformMatrix4fv(shader.projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

		mMat = glm::mat4(1.0f);
		mMat = glm::translate(mMat, glm::vec3(0.0f, 445.0f, 0));
		mvMat = vMat * mMat;
		glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
		glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glassrect1.RenderMesh();


		mMat = glm::mat4(1.0f);
		mMat = glm::translate(mMat, glm::vec3(0.0f, 230.0f, -295.0f));
		mvMat = vMat * mMat;
		glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
		glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glassrect2.RenderMesh();

		mMat = glm::mat4(1.0f);
		mMat = glm::translate(mMat, glm::vec3(800.0f, 650.0f, 0.0f));
		mvMat = vMat * mMat;
		glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
		glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glassrect3.RenderMesh();

		glUniform1i(shader.isglass, 0);
		glDisable(GL_BLEND);
	}

	if (!shadowpass)
	{
		if (editormode)
		{
			glUseProgram(shader.program);
			glUniform1i(shader.ishitbox, 1);

			/*HitBoxes*/
			for (int i = 0; i < staticHitBoxes.size(); i++)
			{
				mMat = glm::mat4(1.0f);
				mMat = glm::translate(mMat, staticHitBoxes[i].center);
				mvMat = vMat * mMat;
				glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
				glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
				staticHitBoxes[i].model->RenderMesh();
			}
			mMat = glm::mat4(1.0f);
			mMat = glm::translate(mMat, player1.hitbox.center);
			mvMat = vMat * mMat;
			glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
			glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
			//player1.hitbox.model->RenderMesh();

			/*HitPlanes*/
			for (int i = 0; i < staticHitPlanes.size(); i++)
			{
				mMat = glm::mat4(1.0f);
				mvMat = vMat * mMat;
				glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
				glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
				staticHitPlanes[i].model->RenderMesh();
				for (int j = 0; j < staticHitPlanes[i].hitboxes.size(); j++)
				{
					mMat = glm::mat4(1.0f);
					mMat = glm::translate(mMat, staticHitPlanes[i].hitboxes[j].center);
					mvMat = vMat * mMat;
					glUniformMatrix4fv(shader.mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
					glUniformMatrix4fv(shader.mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
					staticHitPlanes[i].hitboxes[j].model->RenderMesh();
				}
			}
			glUniform1i(shader.ishitbox, 0);
		}
	}
}

int InitializeGLFWandGLEW()
{
	srand(time(0));
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	width = 1040;
	height = 680;
	window = glfwCreateWindow(1040, 680, "HeartMan Shooter", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	//glfwSwapInterval(1);

	cout << glGetString(GL_VERSION) << endl;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool InitializeSDL()
{

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	sound_akfiring = Mix_LoadMUS("Sounds/akloop.wav");
	sound_akreload = Mix_LoadWAV("Sounds/ak47reload.wav");
	sound_aklastbullet = Mix_LoadWAV("Sounds/aklastbullet.wav");
	sound_pistolshot = Mix_LoadWAV("Sounds/pistolshot.wav");
	sound_pistolreload = Mix_LoadWAV("Sounds/pistolreload.wav");
	sound_awpshot = Mix_LoadWAV("Sounds/awpshot.wav");
	sound_awpreload = Mix_LoadWAV("Sounds/awpreload.wav");

	return true;
}

void close()
{
	/*Free all SDL_Mixer stuff*/
	Mix_FreeChunk(sound_akreload);
	sound_akfiring = nullptr;
	Mix_FreeChunk(sound_aklastbullet);
	sound_aklastbullet = nullptr;
	Mix_FreeChunk(sound_pistolshot);
	sound_pistolshot = nullptr;
	Mix_FreeChunk(sound_pistolreload);
	sound_pistolreload = nullptr;
	Mix_FreeChunk(sound_awpshot);
	sound_awpshot = nullptr;
	Mix_FreeChunk(sound_awpreload);
	sound_awpreload = nullptr;

	Mix_FreeMusic(sound_akfiring);
	sound_akfiring = nullptr;

	Mix_Quit();

	CrashB.ClearModel();
	Tree.ClearModel();
	Helicopter.ClearModel();
	Nanosuit.ClearModel();
	AwpMap.ClearModel();
	Mario.ClearModel();
	WorldMap.ClearModel();
	World.ClearModel();
	Bullet.ClearModel();
	PistolBullet.ClearModel();
	Flash.ClearModel();	
}

void HandleEditorInput()
{
	float hitboxmove = 0.1f;
	if (globalkeys[GLFW_KEY_U])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Translate(0, hitboxmove, 0);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].center.y += hitboxmove;
		}
	}
	if (globalkeys[GLFW_KEY_O])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Translate(0, -hitboxmove, 0);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].center.y -= hitboxmove;
		}
	}
	if (globalkeys[GLFW_KEY_I])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Translate(0, 0, -hitboxmove);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].center.z -= hitboxmove;
		}
	}
	if (globalkeys[GLFW_KEY_K])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Translate(0, 0, hitboxmove);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].center.z += hitboxmove;
		}
	}
	if (globalkeys[GLFW_KEY_J])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Translate(-hitboxmove, 0, 0);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].center.x -= hitboxmove;
		}
	}
	if (globalkeys[GLFW_KEY_L])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Translate(hitboxmove, 0, 0);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].center.x += hitboxmove;
		}
	}

	GLfloat moveamount = .1f;
	if (globalkeys[GLFW_KEY_SLASH])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Scale(moveamount, 0, 0);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].width += moveamount;
			staticHitBoxes[hitboxindex].GenerateNewModel();
		}
	}
	if (globalkeys[GLFW_KEY_PERIOD])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Scale(-moveamount, 0, 0);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].width -= moveamount;
			if (staticHitBoxes[hitboxindex].width < 0)
			{
				staticHitBoxes[hitboxindex].width += moveamount;
			}
			staticHitBoxes[hitboxindex].GenerateNewModel();
		}
	}
	if (globalkeys[GLFW_KEY_RIGHT_BRACKET])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Scale(0, 0, moveamount);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].thedepth += moveamount;
			staticHitBoxes[hitboxindex].GenerateNewModel();
		}
	}
	if (globalkeys[GLFW_KEY_LEFT_BRACKET])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Scale(0, 0, -moveamount);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].thedepth -= moveamount;
			if (staticHitBoxes[hitboxindex].thedepth < 0)
			{
				staticHitBoxes[hitboxindex].thedepth += moveamount;
			}
			staticHitBoxes[hitboxindex].GenerateNewModel();
		}
	}
	if (globalkeys[GLFW_KEY_SEMICOLON])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Scale(0, -moveamount, 0);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].height -= moveamount;
			if (staticHitBoxes[hitboxindex].height < 0)
			{
				staticHitBoxes[hitboxindex].height += moveamount;
			}
			staticHitBoxes[hitboxindex].GenerateNewModel();
		}
	}
	if (globalkeys[GLFW_KEY_APOSTROPHE])
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Scale(0, moveamount, 0);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
		else
		{
			staticHitBoxes[hitboxindex].height += moveamount;
			staticHitBoxes[hitboxindex].GenerateNewModel();
		}
	}

	if (singlekeys[GLFW_KEY_M].key)
	{
		if (!hitboxon)
		{
			if (hitplaneindex < staticHitPlanes.size() - 1)
				hitplaneindex++;
			cout << "plane index: " << hitplaneindex << endl;
		}
		else
		{
			if (hitboxindex < staticHitBoxes.size() - 1)
				hitboxindex++;
			cout << "box index: " << hitboxindex << endl;
		}	
	}
	if (singlekeys[GLFW_KEY_N].key)
	{
		if (!hitboxon)
		{
			if (hitplaneindex > 0)
				hitplaneindex--;
			cout << "plane index: " << hitplaneindex << endl;
		}
		else
		{
			if (hitboxindex > 0)
				hitboxindex--;
			cout << "box index: " << hitboxindex << endl;
		}	
	}
	if (singlekeys[GLFW_KEY_P].key)
	{
		currentcamera = &camera;
		cout << "Printing hitbox info to file..." << endl;
		fstream file;
		file.open("Hitboxdata.txt", ios::out);
		for (int i = 0; i < staticHitBoxes.size(); i++)
		{
			file << "staticHitBoxes.emplace_back(glm::vec3(" << staticHitBoxes[i].center.x << "," << staticHitBoxes[i].center.y << "," << staticHitBoxes[i].center.z << ") ";
			file << ", " << staticHitBoxes[i].width << ", " << staticHitBoxes[i].height << ", " << staticHitBoxes[i].thedepth <<", editormode);"<< endl;
		}
		file.close();

		cout << "Printing hitplane info to file..." << endl;
		fstream file2;
		file2.open("Hitplanedata.txt", ios::out);
		for (int i = 0; i < staticHitPlanes.size(); i++)
		{
			file2 << "index " << i << ":";
			file2 << "glm::vec3(" << staticHitPlanes[i].pointfl.x << "," << staticHitPlanes[i].pointfl.y << "," << staticHitPlanes[i].pointfl.z << "), ";
			file2 << " glm::vec3(" << staticHitPlanes[i].pointfr.x << "," << staticHitPlanes[i].pointfr.y << "," << staticHitPlanes[i].pointfr.z << "), ";
			file2 << " glm::vec3(" << staticHitPlanes[i].pointbl.x << "," << staticHitPlanes[i].pointbl.y << "," << staticHitPlanes[i].pointbl.z << "), ";
			file2 << " glm::vec3(" << staticHitPlanes[i].pointbr.x << "," << staticHitPlanes[i].pointbr.y << "," << staticHitPlanes[i].pointbr.z << "), ";
			file2 << "editormode,";
			file2 << " glm::vec3(" << staticHitPlanes[i].center.x << "," << staticHitPlanes[i].center.y << "," << staticHitPlanes[i].center.z << "), ";
			file2 << " " << staticHitPlanes[i].angle;
			file2 << endl;
		}
		file2.close();
	}

	if (singlekeys[GLFW_KEY_GRAVE_ACCENT].key)
	{
		noclip = !noclip;
	}

	if (singlekeys[GLFW_KEY_R].key)
	{
		if (!hitboxon)
		{
			staticHitPlanes[hitplaneindex].Rotate(90);
			staticHitPlanes[hitplaneindex].GenerateNewModel();
		}
	}
	if (singlekeys[GLFW_KEY_Y].key)
	{
		hitboxon = !hitboxon;
	}
}

void SetPerspectiveMatrix()
{
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 2000.0f);//1.0472f
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			globalkeys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			globalkeys[key] = false;
		}
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS && singlekeys[key].down == false)
		{
			singlekeys[key].key = true;
			singlekeys[key].down = true;
		}
		else if(action == GLFW_RELEASE)
		{
			singlekeys[key].key = false;
			singlekeys[key].down = false;
		}
		else
		{
			singlekeys[key].key = false;
		}
	}
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		rightclick = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		rightclick = false;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		leftclick = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		leftclick = false;
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void MouseHandler(float& x, float& y)
{
	double xx, yy;
	glfwGetCursorPos(window, &xx, &yy);

	assert(xx != NULL || yy != NULL, "cursor position overflow");

	if (mouseFirstMoved)
	{
		mousex = xx;
		mousey = yy;
		mouseFirstMoved = false;
	}

	x = xx - mousex;
	y = mousey - yy;

	mousex = xx;
	mousey = yy;
}

void SetupMeshes()
{
	/*Load Models*/
	//CrashB.LoadModel("Models/crashbandicoot.obj");
	//Tree.LoadModel("Models/Tree1.obj");
	//Helicopter.LoadModel("Models/uh60.obj");
	//Nanosuit.LoadModel("Models/nanosuit.obj");
	AwpMap.LoadModel("Models/awp_india.obj");
	Mario.LoadModel("Models/Mario.obj");
	WorldMap.LoadModel("Models/map.obj");//map2 smooth normals
	Bullet.LoadModel("Models/bullet.obj");
	PistolBullet.LoadModel("Models/bullet.obj");
	Flash.LoadModel("Models/flash.obj");
	Dino.LoadModel("Models/dino.obj");//dino
	World.LoadModel("Models/WorldMap.obj");
	theTorch.LoadModel("Models/torch.obj");

	Ak47.CreateAk47();
	Awp.CreateAwp();
	Pistol.CreatePistol();
	
	/*Change data*/
	//Nanosuit.ChangeandUploadData(Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), Utils::ToRadians(90)));

	Bullet.ChangeandUploadData(glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f)));
	Bullet.ChangeandUploadData(Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), Utils::ToRadians(180)));

	PistolBullet.ChangeandUploadData(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
	PistolBullet.ChangeandUploadData(Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), Utils::ToRadians(180)));

	Mario.ChangeandUploadData(glm::scale(glm::mat4(1.0f), glm::vec3(4.5f, 4.5f, 4.5f)));
	Mario.ChangeandUploadData(Utils::GetRodriguezMatrix(glm::vec3(1.0f, 0.0f, 0.0f), Utils::ToRadians(-90)));
	Mario.ChangeandUploadData(Utils::GetRodriguezMatrix(glm::vec3(0.0f, 1.0f, 0.0f), Utils::ToRadians(-90)));

	World.ChangeandUploadData(glm::scale(glm::mat4(1.0f), glm::vec3(15.0f, 15.0f, 15.0f)));


	/*World Triangles*/
	for (int i = 0; i < World.meshList.size(); i++)
	{
		for (int j = 0; j < World.meshList[i]->indexdatasize; j+=3)
		{
			//grab next 3 indices and use those offsets for worldmap
			Triangle tmp;
			tmp.SetTriangle(World.meshList[i]->meshdata[World.meshList[i]->indexdata[j] * 8 + 0], World.meshList[i]->meshdata[World.meshList[i]->indexdata[j] * 8 + 1], World.meshList[i]->meshdata[World.meshList[i]->indexdata[j] * 8 + 2],
				World.meshList[i]->meshdata[World.meshList[i]->indexdata[j + 1] * 8 + 0], World.meshList[i]->meshdata[World.meshList[i]->indexdata[j + 1] * 8 + 1], World.meshList[i]->meshdata[World.meshList[i]->indexdata[j + 1] * 8 + 2],
				World.meshList[i]->meshdata[World.meshList[i]->indexdata[j + 2] * 8 + 0], World.meshList[i]->meshdata[World.meshList[i]->indexdata[j + 2] * 8 + 1], World.meshList[i]->meshdata[World.meshList[i]->indexdata[j + 2] * 8 + 2]);
			WorldTriangles.push_back(tmp);
		}
	}
}

void CreateStaticHitboxes()
{
	//default staticHitBoxes.emplace_back(glm::vec3(0, 0, 0), 5, 5, 5, editormode);
	staticHitBoxes.reserve(41);

	staticHitBoxes.emplace_back(glm::vec3(2.1, -33.7001, -191.302), 420.816, 38.7, 115.099, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-34.3001, -61.1996, 184.502), 412.316, 66.3996, 104.399, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-288.708, -20.7, 2.8), 135.799, 26.1001, 79.2994, editormode);
	staticHitBoxes.emplace_back(glm::vec3(250.406, 0.5, 2.3), 198.003, 5, 79.7994, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-54.2997, -8.9, 5.2), 121.099, 5, 90.0992, editormode);
	staticHitBoxes.emplace_back(glm::vec3(67.7995, 10.6, -244.606), 32.5001, 99.3991, 32.4001, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-39.3, 32.8001, -247.806), 32.8001, 37, 36.2, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-101.099, 10, -247.006), 37, 31.0001, 35.2, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-167.701, 9, -249.006), 32.8001, 14.4, 37, editormode);
	staticHitBoxes.emplace_back(glm::vec3(70.4995, 18.9, 239.505), 16, 19.8, 32.0001, editormode);
	staticHitBoxes.emplace_back(glm::vec3(40.5, 20.4, 252.706), 15, 16.1, 15.9, editormode);
	staticHitBoxes.emplace_back(glm::vec3(70.6995, 57.1997, 254.306), 15.5, 7.8, 17.4, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-215.304, 9.2, 195.103), 46.6999, 5, 45.8999, editormode);
	staticHitBoxes.emplace_back(glm::vec3(0, 202.603, 308.91), 390.715, 248.106, 34.9, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-418.516, 219.804, -6.49999), 31.2001, 233.205, 287.008, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-12.5, 195.703, -300.209), 415.916, 246.906, 23.7001, editormode);
	staticHitBoxes.emplace_back(glm::vec3(405.116, 216.004, -191.402), 16.8, 229.205, 114.799, editormode);
	staticHitBoxes.emplace_back(glm::vec3(405.416, 216.404, 189.302), 16.7, 226.405, 108.599, editormode);
	staticHitBoxes.emplace_back(glm::vec3(412.516, 226.505, 7.19999), 23.6001, 41.9999, 87.7992, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-52.9998, 44.7999, 0), 29.3001, 4.3, 76.7994, editormode);
	staticHitBoxes.emplace_back(glm::vec3(248.306, 4, -96.6991), 15.3, 68.1995, 9.5, editormode);
	staticHitBoxes.emplace_back(glm::vec3(251.306, 30.5001, -139.099), 13.2, 26.3001, 8.5, editormode);
	staticHitBoxes.emplace_back(glm::vec3(248.206, 6.1, 143.1), 15.4, 45.5999, 8.1, editormode);
	staticHitBoxes.emplace_back(glm::vec3(251.706, 12.8, 99.2991), 12.3, 35.2, 10, editormode);
	staticHitBoxes.emplace_back(glm::vec3(333.911, 15.5, 227.505), 29.3001, 12.6, 29.0001, editormode);
	staticHitBoxes.emplace_back(glm::vec3(333.711, 70.7995, 227.405), 14.2, 81.7993, 13.9, editormode);
	staticHitBoxes.emplace_back(glm::vec3(326.111, 65.0996, -226.005), 14.5, 59.0997, 14.8, editormode);
	staticHitBoxes.emplace_back(glm::vec3(326.311, 11.9, -225.905), 28.3001, 16.1, 28.9001, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-328.611, 15.6, -231.205), 29.2001, 12.4, 28.3001, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-328.411, 65.2996, -231.705), 14, 63.4996, 16.2, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-342.112, 14.9, 238.705), 29.1001, 12.5, 30.0001, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-341.812, 78.3994, 236.605), 15.3, 54.5997, 14.6, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-111.299, -0.100002, -1.9), 16.6, 9.2, 22.5001, editormode);
	staticHitBoxes.emplace_back(glm::vec3(16.6, 2.9, 2.6), 16.6, 6.5, 17.9, editormode);
	staticHitBoxes.emplace_back(glm::vec3(552.709, -335.611, 9.5), 128.999, 335.111, 368.213, editormode);
	staticHitBoxes.emplace_back(glm::vec3(773.155, -369.513, -230.805), 93.3992, 368.913, 138.999, editormode);
	staticHitBoxes.emplace_back(glm::vec3(771.756, -342.412, 234.705), 92.2992, 342.112, 146.4, editormode);
	staticHitBoxes.emplace_back(glm::vec3(1004.1, -293.109, 5.2), 141.199, 292.509, 366.013, editormode);
	staticHitBoxes.emplace_back(glm::vec3(767.257, 191.702, -357.313), 367.213, 439.918, 5, editormode);
	staticHitBoxes.emplace_back(glm::vec3(776.155, 39.8, 368.713), 357.913, 247.806, 16.6, editormode);
	staticHitBoxes.emplace_back(glm::vec3(1143.56, -1.5, -1.2), 18.9, 323.711, 358.013, editormode);





	/*staticHitBoxes.emplace_back(glm::vec3(100.099, 13.7, -85.9993), 7.6, 8.5, 8, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-62.7996, 19.3, -87.6992), 12.3, 11.2, 14.2, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-101.199, 53.3998, -86.6992), 11.6, 1.2, 11.7, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-70.4995, 18.5, 46.1999), 11.5, 12.7, 13, editormode);
	staticHitBoxes.emplace_back(glm::vec3(98.0991, 7, 104.699), 30.5001, 2.5, 25.3001, editormode);
	staticHitBoxes.emplace_back(glm::vec3(0, 50.2998, -141.199), 145.9, 43.2999, 2.2, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-140.299, 45.0999, 1.8), 1.2, 48.8998, 140.299, editormode);
	staticHitBoxes.emplace_back(glm::vec3(-1.3, 48.7998, 140.399), 139.799, 45.3999, 1.2, editormode);
	staticHitBoxes.emplace_back(glm::vec3(137.899, 45.3999, -2.8), 1.4, 47.7998, 142.199, editormode);
	staticHitBoxes.emplace_back(glm::vec3(4.4, -9.29999, 0), 155.1, 16.2, 153.6, editormode);
	staticHitBoxes.emplace_back(glm::vec3(5.9, 48.2999, 7.4), 28.9001, 0.0999996, 22.9001, editormode);*/

}

void CreateStaticPlanes()
{
	//default staticHitPlanes.emplace_back(glm::vec3(-10,0,0),glm::vec3(10,0,0),glm::vec3(-10,5,-10),glm::vec3(10,5,-10),editormode, glm::vec3(0,0,0),0);
	staticHitPlanes.reserve(2);

	staticHitPlanes.emplace_back(glm::vec3(-81.8993, 5.1, 158.605), glm::vec3(-23.9004, 5.1, 158.796), glm::vec3(-81.8037, 49.3, 76.8049), glm::vec3(-23.8048, 49.3, 76.9959), editormode, glm::vec3(-52.8998, 5.1, 158.7), 0);
	staticHitPlanes.emplace_back(glm::vec3(-24.2003, 3.9, -164.455), glm::vec3(-82.3995, 3.9, -164.547), glm::vec3(-24.3403, 49.1998, -76.549), glm::vec3(-82.5402, 49.1998, -76.6407), editormode, glm::vec3(-53.2998, 3.9, -164.501), 180);


	/*staticHitPlanes.emplace_back(glm::vec3(25.2011, 2.4, -99.5689), glm::vec3(-12.6009, 2.4, -99.6291), glm::vec3(25.0671, 48.0998, -15.6665), glm::vec3(-12.7346, 48.0998, -15.7272), editormode, glm::vec3(6.3, 2.4, -99.599), 180);
	staticHitPlanes.emplace_back(glm::vec3(-12.3, 2.1, 111.399), glm::vec3(24.7001, 2.1, 111.399), glm::vec3(-12.3, 49.4998, 31.6001), glm::vec3(24.7001, 49.4998, 31.6001), editormode, glm::vec3(6.2, 2.1, 111.399), 0);*/
}

void HandleButtons()
{
	if (editormode)
	{
		if (singlekeys[GLFW_KEY_LEFT].key)
		{
			camera.yaw--;
		}
		if (singlekeys[GLFW_KEY_RIGHT].key)
		{
			camera.yaw++;
		}
		if (globalkeys[GLFW_KEY_Z])
		{
			camerafreelock = !camerafreelock;
		}
		if (globalkeys[GLFW_KEY_1])
		{
			currentcamera = &camera;
		}
		if (globalkeys[GLFW_KEY_2])
		{
			currentcamera = &camera3Person;
		}
		if (globalkeys[GLFW_KEY_3])
		{
			currentcamera = &cameraFree;
		}
		if (globalkeys[GLFW_KEY_V])
		{
			player1.SwapWeapon();
			player1.weapon = &Awp;
		}
		if (globalkeys[GLFW_KEY_B])
		{
			player1.SwapWeapon();
			player1.weapon = &Ak47;
		}
		HandleEditorInput();
	}
	else
	{
		float pointvalue = .1f;
		if (globalkeys[GLFW_KEY_I])
		{
			point1position.z+=pointvalue;
			diffuse1.direction.z++;
			shadow_diffuse1.CreateLightMatrix(diffuse1);
		}
		if (globalkeys[GLFW_KEY_K])
		{
			point1position.z -= pointvalue;
			diffuse1.direction.z--;
			shadow_diffuse1.CreateLightMatrix(diffuse1);
		}
		if (globalkeys[GLFW_KEY_J])
		{
			point1position.x -= pointvalue;
			diffuse1.direction.x--;
			shadow_diffuse1.CreateLightMatrix(diffuse1);
		}
		if (globalkeys[GLFW_KEY_L])
		{
			point1position.x += pointvalue;
			diffuse1.direction.x++;
			shadow_diffuse1.CreateLightMatrix(diffuse1);
		}
		if (globalkeys[GLFW_KEY_U])
		{
			point1position.y += pointvalue;
			diffuse1.direction.y++;
			shadow_diffuse1.CreateLightMatrix(diffuse1);
		}
		if (globalkeys[GLFW_KEY_O])
		{
			point1position.y -= pointvalue;
			diffuse1.direction.y--;
			shadow_diffuse1.CreateLightMatrix(diffuse1);
		}
		if (globalkeys[GLFW_KEY_1])
		{
			currentcamera = &camera;
		}
		if (globalkeys[GLFW_KEY_2])
		{
			currentcamera = &camera3Person;
		}
		if (globalkeys[GLFW_KEY_3])
		{
			currentcamera = &cameraFree;
		}
		if (globalkeys[GLFW_KEY_Z])
		{
			camerafreelock = !camerafreelock;
		}
		if (globalkeys[GLFW_KEY_C])
		{
			player1.SwapWeapon();
			player1.weapon = &Pistol;
			shadow_diffuse1.lightdistance--;
			shadow_diffuse1.CreateLightMatrix(diffuse1);
		}
		if (globalkeys[GLFW_KEY_V])
		{
			player1.SwapWeapon();
			player1.weapon = &Awp;
			shadow_diffuse1.lightdistance++;
			shadow_diffuse1.CreateLightMatrix(diffuse1);
		}
		if (globalkeys[GLFW_KEY_B])
		{
			player1.SwapWeapon();
			player1.weapon = &Ak47;
		}
		if (globalkeys[GLFW_KEY_M])
		{
		}
		if (globalkeys[GLFW_KEY_N])
		{
		}
		if (singlekeys[GLFW_KEY_GRAVE_ACCENT].key)
		{
			noclip = !noclip;
		}
		if (globalkeys[GLFW_KEY_5])
		{
			camera.pitch--;
		}
		if (globalkeys[GLFW_KEY_6])
		{
			camera.pitch++;
		}
	}
}

void ChangeLevels(int level)
{
	//switch LEVEL and switch shadow arrays
	if (level == 0)
	{
		LEVEL = 0;
		shadermain.pointloop = 7;
		shadermain.spotloop = 1;
		shadermain.dirloop = 1;
		shadermain.pointshadows = 0;
	}
	if (level == 1)
	{
		LEVEL = 1;

		//shadow_Diffuse.clear();
		//shadow_Diffuse.push_back(&shadow_diffuse1);

		//shadow_Spot.clear();
		//shadow_Spot.push_back(&shadow_spot1);


		//shadow_Point.clear();
		//shadow_Point.push_back(&shadow_point1);
		shadermain.pointloop = 0;
		shadermain.spotloop = 0;
		shadermain.dirloop = 0;
		shadermain.pointshadows = 1;
	}
}