#include "Shader.h"

Shader::Shader()
{

}

Shader::Shader(const char* vertex, const char* frag)
{
	createShaderProgram(vertex, frag);
}

void Shader::createShaderProgram(const char* vertex, const char* frag, const char* geom)
{
	std::string shadersource = ReadFile(vertex);
	std::string fragsource = ReadFile(frag);
	std::string geomsource = ReadFile(geom);
	const char *vshadersource = shadersource.c_str();
	const char *fshadersource = fragsource.c_str();
	const char *gshadersource = geomsource.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint gShader = glCreateShader(GL_GEOMETRY_SHADER);

	glShaderSource(vShader, 1, &vshadersource, NULL);
	glShaderSource(fShader, 1, &fshadersource, NULL);
	glShaderSource(gShader, 1, &gshadersource, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	glCompileShader(gShader);

	GLint compiled;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "vertex shader failed to compile!" << std::endl;
		printShaderlog(vShader);
	}
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "fragment shader failed to compile!" << std::endl;
		printShaderlog(fShader);
	}
	glGetShaderiv(gShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "geometry shader failed to compile!" << std::endl;
		printShaderlog(gShader);
	}

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glAttachShader(program, gShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "Program linking failed!" << std::endl;
		printProgramlog(program);
	}

	Utils::checkOpenGLError();

	mvLoc = glGetUniformLocation(program, "mv_matrix");
	projLoc = glGetUniformLocation(program, "proj_matrix");
	tfLoc = glGetUniformLocation(program, "tf");
	vLoc = glGetUniformLocation(program, "v_matrix");
	mLoc = glGetUniformLocation(program, "m_matrix");
	textsample = glGetUniformLocation(program, "textsample");
	ishitbox = glGetUniformLocation(program, "ishitbox");
	isflash = glGetUniformLocation(program, "isflash");
	textureused = glGetUniformLocation(program, "textureused");
	lightPos = glGetUniformLocation(program, "lightPos");
	far_plane = glGetUniformLocation(program, "far_plane");
	pointMatrix[0] = glGetUniformLocation(program, "pointMatrices[0]");
	pointMatrix[1] = glGetUniformLocation(program, "pointMatrices[1]");
	pointMatrix[2] = glGetUniformLocation(program, "pointMatrices[2]");
	pointMatrix[3] = glGetUniformLocation(program, "pointMatrices[3]");
	pointMatrix[4] = glGetUniformLocation(program, "pointMatrices[4]");
	pointMatrix[5] = glGetUniformLocation(program, "pointMatrices[5]");

	glGetError();
}

void Shader::createShaderProgram(const char* vertex, const char* frag, const char* tcs, const char* tes)
{
	std::string shadersource = ReadFile(vertex);
	std::string fragsource = ReadFile(frag);
	std::string tcssource = ReadFile(tcs);
	std::string tessource = ReadFile(tes);
	const char *vshadersource = shadersource.c_str();
	const char *fshadersource = fragsource.c_str();
	const char *tcsshadersource = tcssource.c_str();
	const char *tesshadersource = tessource.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint tcsShader = glCreateShader(GL_TESS_CONTROL_SHADER);
	GLuint tesShader = glCreateShader(GL_TESS_EVALUATION_SHADER);

	glShaderSource(vShader, 1, &vshadersource, NULL);
	glShaderSource(fShader, 1, &fshadersource, NULL);
	glShaderSource(tcsShader, 1, &tcsshadersource, NULL);
	glShaderSource(tesShader, 1, &tesshadersource, NULL);
	
	glCompileShader(vShader);
	glCompileShader(fShader);
	glCompileShader(tcsShader);
	glCompileShader(tesShader);

	GLint compiled;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "vertex shader failed to compile!" << std::endl;
		printShaderlog(vShader);
	}
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "fragment shader failed to compile!" << std::endl;
		printShaderlog(fShader);
	}
	glGetShaderiv(tcsShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "tesselator control shader failed to compile!" << std::endl;
		printShaderlog(tcsShader);
	}
	glGetShaderiv(tesShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "tesselator cvaluator shader failed to compile!" << std::endl;
		printShaderlog(tesShader);
	}

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, tcsShader);
	glAttachShader(program, tesShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "Program linking failed!" << std::endl;
		printProgramlog(program);
	}

	Utils::checkOpenGLError();


	mvLoc = glGetUniformLocation(program, "mv_matrix");
	projLoc = glGetUniformLocation(program, "proj_matrix");
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}
void Shader::printShaderlog(GLuint shader)
{
	int len;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, 0, log);
		std::cout << "Shader info log: " << log << std::endl;
		free(log);
	}
}
void Shader::printProgramlog(GLuint program)
{
	int len;
	char* log;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetProgramInfoLog(program, len, 0, log);
		std::cout << "Program info log: " << log << std::endl;
		free(log);
	}
}
void Shader::createShaderProgram(const char* vertex, const char* frag)
{
	std::string shadersource = ReadFile(vertex);
	std::string fragsource = ReadFile(frag);
	const char *vshadersource = shadersource.c_str();
	const char *fshadersource = fragsource.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vshadersource, NULL);
	glShaderSource(fShader, 1, &fshadersource, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);

	GLint compiled;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "vertex shader failed to compile!" << std::endl;
		printShaderlog(vShader);
	}
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "fragment shader failed to compile!" << std::endl;
		printShaderlog(fShader);
	}

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &compiled);
	if (compiled != 1)
	{
		std::cout << "Program linking failed!" << std::endl;
		printProgramlog(program);
	}

	Utils::checkOpenGLError();

	mvLoc = glGetUniformLocation(program, "mv_matrix");
	projLoc = glGetUniformLocation(program, "proj_matrix");
	tfLoc = glGetUniformLocation(program, "tf");
	vLoc = glGetUniformLocation(program, "v_matrix");
	mLoc = glGetUniformLocation(program, "m_matrix");
	textsample = glGetUniformLocation(program, "textsample");
	ishitbox = glGetUniformLocation(program, "ishitbox");
	isflash = glGetUniformLocation(program, "isflash");
	textureused = glGetUniformLocation(program, "textureused");
	eyeposition = glGetUniformLocation(program, "eyeposition");
	ambient = glGetUniformLocation(program, "material.ambient");
	diffuse = glGetUniformLocation(program, "material.diffuse");
	specular = glGetUniformLocation(program, "material.specular");
	shininess = glGetUniformLocation(program, "material.shininess");
	lightmatrix = glGetUniformLocation(program, "lightmatrix");
	depthmap = glGetUniformLocation(program, "depthmap");
	renderdepthmap = glGetUniformLocation(program, "renderdepthmap");
	lightPos = glGetUniformLocation(program, "lightPos");
	far_plane = glGetUniformLocation(program, "far_plane");
	pointdepthmap = glGetUniformLocation(program, "pointdepthmap");
	clipping = glGetUniformLocation(program, "clipping");
	clip_plane = glGetUniformLocation(program, "clip_plane");
	dudvmap = glGetUniformLocation(program, "dudvmap");
	movefactor = glGetUniformLocation(program, "movefactor");
	cameraposition = glGetUniformLocation(program, "cameraposition");
	renderingwater = glGetUniformLocation(program, "renderingwater");
	normalmap = glGetUniformLocation(program, "normalmap");
	isglass = glGetUniformLocation(program, "isglass");
	pointloop = glGetUniformLocation(program, "pointlightloop");
	spotloop = glGetUniformLocation(program, "spotlightloop");
	dirloop = glGetUniformLocation(program, "directionallightloop");
	pointshadows = glGetUniformLocation(program, "pointshadows");

	pointMatrix[0] = glGetUniformLocation(program, "pointMatrices[0]");
	pointMatrix[1] = glGetUniformLocation(program, "pointMatrices[1]");
	pointMatrix[2] = glGetUniformLocation(program, "pointMatrices[2]");
	pointMatrix[3] = glGetUniformLocation(program, "pointMatrices[3]");
	pointMatrix[4] = glGetUniformLocation(program, "pointMatrices[4]");
	pointMatrix[5] = glGetUniformLocation(program, "pointMatrices[5]");

	diffusedepthmaps[0] = glGetUniformLocation(program, "diffusedepthmap[0]");
	diffusedepthmaps[1] = glGetUniformLocation(program, "diffusedepthmap[1]");
	diffusedepthmaps[2] = glGetUniformLocation(program, "diffusedepthmap[2]");
	diffusedepthmaps[3] = glGetUniformLocation(program, "diffusedepthmap[3]");
	diffusedepthmaps[4] = glGetUniformLocation(program, "diffusedepthmap[4]");
	diffusedepthmaps[5] = glGetUniformLocation(program, "diffusedepthmap[5]");

	diffusematrix[0] = glGetUniformLocation(program, "diffuseshadowmatrixes[0]");
	diffusematrix[1] = glGetUniformLocation(program, "diffuseshadowmatrixes[1]");
	diffusematrix[2] = glGetUniformLocation(program, "diffuseshadowmatrixes[2]");
	diffusematrix[3] = glGetUniformLocation(program, "diffuseshadowmatrixes[3]");
	diffusematrix[4] = glGetUniformLocation(program, "diffuseshadowmatrixes[4]");
	diffusematrix[5] = glGetUniformLocation(program, "diffuseshadowmatrixes[5]");

	spotdepthmaps[0] = glGetUniformLocation(program, "spotdepthmap[0]");
	spotdepthmaps[1] = glGetUniformLocation(program, "spotdepthmap[1]");
	spotdepthmaps[2] = glGetUniformLocation(program, "spotdepthmap[2]");
	spotdepthmaps[3] = glGetUniformLocation(program, "spotdepthmap[3]");
	spotdepthmaps[4] = glGetUniformLocation(program, "spotdepthmap[4]");
	spotdepthmaps[5] = glGetUniformLocation(program, "spotdepthmap[5]");

	spotmatrix[0] = glGetUniformLocation(program, "spotshadowmatrixes[0]");
	spotmatrix[1] = glGetUniformLocation(program, "spotshadowmatrixes[1]");
	spotmatrix[2] = glGetUniformLocation(program, "spotshadowmatrixes[2]");
	spotmatrix[3] = glGetUniformLocation(program, "spotshadowmatrixes[3]");
	spotmatrix[4] = glGetUniformLocation(program, "spotshadowmatrixes[4]");
	spotmatrix[5] = glGetUniformLocation(program, "spotshadowmatrixes[5]");

	pointdepthmaps[0] = glGetUniformLocation(program, "pointdepthmap[0]");
	pointdepthmaps[1] = glGetUniformLocation(program, "pointdepthmap[1]");
	pointdepthmaps[2] = glGetUniformLocation(program, "pointdepthmap[2]");
	pointdepthmaps[3] = glGetUniformLocation(program, "pointdepthmap[3]");
	pointdepthmaps[4] = glGetUniformLocation(program, "pointdepthmap[4]");
	pointdepthmaps[5] = glGetUniformLocation(program, "pointdepthmap[5]");

	pointlightpos[0] = glGetUniformLocation(program, "pointlightpos[0]");
	pointlightpos[1] = glGetUniformLocation(program, "pointlightpos[1]");
	pointlightpos[2] = glGetUniformLocation(program, "pointlightpos[2]");
	pointlightpos[3] = glGetUniformLocation(program, "pointlightpos[3]");
	pointlightpos[4] = glGetUniformLocation(program, "pointlightpos[4]");
	pointlightpos[5] = glGetUniformLocation(program, "pointlightpos[5]");

	pointfar_planes[0] = glGetUniformLocation(program, "pointfar_planes[0]");
	pointfar_planes[1] = glGetUniformLocation(program, "pointfar_planes[1]");
	pointfar_planes[2] = glGetUniformLocation(program, "pointfar_planes[2]");
	pointfar_planes[3] = glGetUniformLocation(program, "pointfar_planes[3]");
	pointfar_planes[4] = glGetUniformLocation(program, "pointfar_planes[4]");
	pointfar_planes[5] = glGetUniformLocation(program, "pointfar_planes[5]");
	
	/*Ambient*/
	ambientcolor = glGetUniformLocation(program, "ambientlight.color");
	ambientintensity = glGetUniformLocation(program, "ambientlight.intensity");

	/*Directional*/
	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "diffuselights[%d].base.color", i);
		uniformdirectionallight[i].uniformColour = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "diffuselights[%d].base.intensity", i);
		uniformdirectionallight[i].uniformAmbientIntensity = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "diffuselights[%d].direction", i);
		uniformdirectionallight[i].uniformDirection = glGetUniformLocation(program, locBuff);
	}

	/*Points*/
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointlights[%d].base.color", i);
		uniformpointlight[i].uniformColour = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointlights[%d].base.intensity", i);
		uniformpointlight[i].uniformAmbientIntensity = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointlights[%d].position", i);
		uniformpointlight[i].uniformPosition = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointlights[%d].constant", i);
		uniformpointlight[i].uniformconstant = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointlights[%d].linear", i);
		uniformpointlight[i].uniformlinear = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointlights[%d].exponent", i);
		uniformpointlight[i].uniformexponent = glGetUniformLocation(program, locBuff);
	}

	/*Spots*/
	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotlights[%d].base.base.color", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotlights[%d].base.base.intensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotlights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotlights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotlights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotlights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotlights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotlights[%d].cutoff", i);
		uniformSpotLight[i].uniformcutoff = glGetUniformLocation(program, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotlights[%d].procedge", i);
		uniformSpotLight[i].uniformprocedge = glGetUniformLocation(program, locBuff);
	}
	Utils::checkOpenGLError();
}

Shader::~Shader()
{
	glDeleteProgram(program);
}