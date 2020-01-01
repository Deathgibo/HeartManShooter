#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "CommonValues.h"
#include "Utils.h"

class Shader
{
public:

	Shader(const char* vertex, const char* frag);
	Shader();
	~Shader();

	std::string ReadFile(const char* fileLocation);
	void printShaderlog(GLuint shader);
	void printProgramlog(GLuint program);
	void createShaderProgram(const char* vertex, const char* frag);
	void createShaderProgram(const char* vertex, const char* frag, const char* tcs, const char* tes);
	void createShaderProgram(const char* vertex, const char* frag, const char* geom);

	GLuint program;
	GLuint mvLoc, projLoc, tfLoc, vLoc, mLoc, textsample, ishitbox, isflash, textureused, eyeposition, lightmatrix, depthmap, renderdepthmap, lightPos,far_plane,
		clipping, clip_plane, dudvmap, movefactor, cameraposition,renderingwater, normalmap, isglass;
	GLuint pointdepthmap;
	GLuint ambient,diffuse,specular, shininess;
	GLuint ambientcolor, ambientintensity;
	GLuint pointloop, spotloop, dirloop, pointshadows;

	GLuint pointMatrix[6];
	GLuint pointdepthmaps[6];
	GLuint pointlightpos[6];
	GLuint pointfar_planes[6];

	GLuint diffusedepthmaps[6];
	GLuint diffusematrix[6];

	GLuint spotdepthmaps[6];
	GLuint spotmatrix[6];

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;

		GLuint uniformDirection;
	}uniformdirectionallight[MAX_DIRECTIONAL_LIGHTS];

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;

		GLuint uniformPosition;
		GLuint uniformconstant;
		GLuint uniformlinear;
		GLuint uniformexponent;
	}uniformpointlight[MAX_POINT_LIGHTS];

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformcutoff;
		GLuint uniformprocedge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];
private:
	

};