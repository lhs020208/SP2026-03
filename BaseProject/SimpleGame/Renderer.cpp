//--------------------------------------------------------
//Renderer.cpp
//--------------------------------------------------------
#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"

#include <ctime>
#include <assert.h>
#include "Windows.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_TriangleShader = CompileShaders("./Shaders/Triangle.vs", "./Shaders/Triangle.glsl");
	m_FSShader = CompileShaders("./Shaders/FS.vs", "./Shaders/FS.glsl");

	//Load Textures
	m_RgbTexture = CreatePngTexture("./Textures/rgb.png", GL_NEAREST);
	m_NumsTexture = CreatePngTexture("./Textures/numbers.png", GL_NEAREST);
	m_ParticleTexture = CreatePngTexture("./Textures/particle.png", GL_NEAREST);
	m_ParticleSpriteTexture = CreatePngTexture("./Textures/particle_sprite.png", GL_NEAREST);
	for (int i = 0; i < 10; i++) {
		char filePath[256];
		sprintf_s(filePath, "Textures/%d.png", i);
		m_NumTexture[i] = CreatePngTexture(filePath, GL_NEAREST);
	}

	//Create VBOs
	CreateVertexBufferObjects();

	// 랜덤 시드
	srand((unsigned int)time(NULL));

	// 필요하면 파티클 생성
	GenParticles(1000);

	int index = 0;
	for (int i = 0; i < 500; i++) {
		float x = (float)rand() / (float)RAND_MAX;
		float y = (float)rand() / (float)RAND_MAX;
		float sTime = 5.0f * (float)rand() / (float)RAND_MAX;
		float lTime = 0.5f * (float)rand() / (float)RAND_MAX;
		m_RainInfo[index] = x; index++;
		m_RainInfo[index] = y; index++;
		m_RainInfo[index] = sTime; index++;
		m_RainInfo[index] = lTime; index++;
	}

	if (m_SolidRectShader > 0 &&
		m_TriangleShader > 0 &&
		m_FSShader > 0 &&
		m_VBORect > 0 &&
		m_VBOFS > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)

{

	//Load Png

	std::vector<unsigned char> image;

	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filePath);

	if (error != 0)

	{

		std::cout << "PNG image loading failed:" << filePath << std::endl;

		assert(0);

	}



	GLuint temp;

	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,

		GL_UNSIGNED_BYTE, &image[0]);



	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);



	return temp;

}

void Renderer::CreateVertexBufferObjects()
{
	float rect[] =
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f,
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,
	};

	glGenBuffers(1, &m_VBORect);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	glGenBuffers(1, &m_VBOTriangle);

	float triangle[] //x, y, z, tx, ty
		=
	{
		-1,-1,0,0,1,	//v0
		1,1,0,1,0,		//v1
		-1,1,0,0,0,		//v2

		-1,-1,0,0,1,	//v0
		1,-1,0,1,1,		//v1
		1,1,0,1,0,		//v2
	};

	glGenBuffers(1, &m_VBOFS);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

}

void Renderer::GenParticles(int num)
{
	m_NumParticles = num;

	float centerX = 0.0f;
	float centerY = 0.0f;
	float size = 0.1f;
	float mass = 1.0f;
	float RV1 = 0.0f;
	float RV2 = 0.0f;
	float RV3 = 0.0f;

	// 정점 하나당: x,y,z,mass,vx,vy,RV1,RV2,RV3, tx, ty, r, g, b -> 14개 float
	// 파티클 하나당 정점 6개
	int totalFloatCount = num * 6 * 14;
	float* particles = new float[totalFloatCount];

	int idx = 0;

	for (int i = 0; i < num; i++)
	{
		float maxSpeed = 1.2f;
		float theta = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.141592f;
		float radius = ((float)rand() / (float)RAND_MAX) * maxSpeed;
		//float vx = radius * cos(theta);
		//float vy = radius * sin(theta) + 1.0f;

		float vx = -1.0f + 2.0f * ((float)rand() / (float)RAND_MAX);
		float vy = -1.0f + 2.0f * ((float)rand() / (float)RAND_MAX);

		float x0 = centerX - size / 2;
		float y0 = centerY - size / 2;
		float x1 = centerX + size / 2;
		float y1 = centerY + size / 2;

		RV1 = (float)rand() / (float)RAND_MAX;
		RV2 = (float)rand() / (float)RAND_MAX;
		RV3 = (float)rand() / (float)RAND_MAX;

		float R = (float)rand() / (float)RAND_MAX;
		float G = (float)rand() / (float)RAND_MAX;
		float B = (float)rand() / (float)RAND_MAX;

		// v0
		particles[idx++] = x0;
		particles[idx++] = y0;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV1;
		particles[idx++] = RV2;
		particles[idx++] = RV3;
		particles[idx++] = 0.f;
		particles[idx++] = 1.f;
		particles[idx++] = R;
		particles[idx++] = G;
		particles[idx++] = B;

		// v1
		particles[idx++] = x1;
		particles[idx++] = y0;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV1;
		particles[idx++] = RV2;
		particles[idx++] = RV3;
		particles[idx++] = 1.f;
		particles[idx++] = 1.f;
		particles[idx++] = R;
		particles[idx++] = G;
		particles[idx++] = B;

		// v2
		particles[idx++] = x1;
		particles[idx++] = y1;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV1;
		particles[idx++] = RV2;
		particles[idx++] = RV3;
		particles[idx++] = 1.f;
		particles[idx++] = 0.f;
		particles[idx++] = R;
		particles[idx++] = G;
		particles[idx++] = B;

		// v3
		particles[idx++] = x0;
		particles[idx++] = y0;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV1;
		particles[idx++] = RV2;
		particles[idx++] = RV3;
		particles[idx++] = 0.f;
		particles[idx++] = 1.f;
		particles[idx++] = R;
		particles[idx++] = G;
		particles[idx++] = B;

		// v4
		particles[idx++] = x1;
		particles[idx++] = y1;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV1;
		particles[idx++] = RV2;
		particles[idx++] = RV3;
		particles[idx++] = 1.f;
		particles[idx++] = 0.f;
		particles[idx++] = R;
		particles[idx++] = G;
		particles[idx++] = B;

		// v5
		particles[idx++] = x0;
		particles[idx++] = y1;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV1;
		particles[idx++] = RV2;
		particles[idx++] = RV3;
		particles[idx++] = 0.f;
		particles[idx++] = 0.f;
		particles[idx++] = R;
		particles[idx++] = G;
		particles[idx++] = B;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totalFloatCount, particles, GL_STATIC_DRAW);

	delete[] particles;
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);

	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float gTime = 0;

void Renderer::DrawTriangle()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gTime += 0.01f;

	glUseProgram(m_TriangleShader);

	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(uTime, gTime);
	int uParticle = glGetUniformLocation(m_TriangleShader, "u_ParticleTex");
	glUniform1i(uParticle, 0);
	int uParticleSprite = glGetUniformLocation(m_TriangleShader, "u_ParticleSpriteTex");
	glUniform1i(uParticleSprite, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ParticleTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ParticleSpriteTexture);

	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");
	int attribRV1 = glGetAttribLocation(m_TriangleShader, "a_RV1");
	int attribRV2 = glGetAttribLocation(m_TriangleShader, "a_RV2");
	int attribRV3 = glGetAttribLocation(m_TriangleShader, "a_RV3");
	int attribTex = glGetAttribLocation(m_TriangleShader, "a_Tex");
	int attribRGB = glGetAttribLocation(m_TriangleShader, "a_RGB");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);
	glEnableVertexAttribArray(attribRV1);
	glEnableVertexAttribArray(attribRV2);
	glEnableVertexAttribArray(attribRV3);
	glEnableVertexAttribArray(attribTex);
	glEnableVertexAttribArray(attribRGB);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);

	int stride = 14;
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), 0);
	glVertexAttribPointer(attribMass, 1, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(attribVel, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(sizeof(float) * 4));
	glVertexAttribPointer(attribRV1, 1, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(attribRV2, 1, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(sizeof(float) * 7));
	glVertexAttribPointer(attribRV3, 1, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(sizeof(float) * 8));
	glVertexAttribPointer(attribTex, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(sizeof(float) * 9));
	glVertexAttribPointer(attribRGB, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, 6 * m_NumParticles);
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribMass);
	glDisableVertexAttribArray(attribVel);
	glDisableVertexAttribArray(attribRV1);
	glDisableVertexAttribArray(attribRV2);
	glDisableVertexAttribArray(attribRV3);
	glDisableVertexAttribArray(attribTex);
	glDisableVertexAttribArray(attribRGB);
}

int g_CurrNum = 0;

void Renderer::DrawFS()
{
	gTime += 0.09f;
	GLuint shader = m_FSShader;
	glUseProgram(shader);

	int uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, gTime);

	int uRGBTexture = glGetUniformLocation(shader, "u_RGBTex");
	glUniform1i(uRGBTexture, 0);
	int uCurrNumTex = glGetUniformLocation(shader, "u_CurrNumTex");
	glUniform1i(uCurrNumTex, g_CurrNum + 2);
	int uNumsTex = glGetUniformLocation(shader, "u_NumsTex");
	glUniform1i(uNumsTex, 1);
	int uinputNum = glGetUniformLocation(shader, "u_inputNum");
	glUniform1i(uinputNum, g_CurrNum);

	g_CurrNum++;
	if (g_CurrNum > 9) g_CurrNum = 0;
	Sleep(200);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RgbTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_NumsTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[0]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[1]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[2]);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[3]);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[4]);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[5]);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[6]);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[7]);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[8]);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, m_NumTexture[9]);

	int uPoints = glGetUniformLocation(shader, "u_Points");
	glUniform4fv(uPoints, 500, m_RainInfo);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	int attribTex = glGetAttribLocation(shader, "a_Tex");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTex);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFS);

	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glVertexAttribPointer(attribTex, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::GetGLPosition(float x, float y, float* newX, float* newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}