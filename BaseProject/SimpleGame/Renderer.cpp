//--------------------------------------------------------
//Renderer.cpp
//--------------------------------------------------------
#include "stdafx.h"
#include "Renderer.h"
#include <ctime>

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
	m_TriangleShader = CompileShaders("./Shaders/Triangle.vs", "./Shaders/Triangle.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	// 랜덤 시드
	srand((unsigned int)time(NULL));

	// 파티클 생성
	GenParticles(100000);

	if (m_SolidRectShader > 0 && m_VBORect > 0 && m_TriangleShader > 0 && m_VBOTriangle > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
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
}

void Renderer::GenParticles(int num)
{
	m_NumParticles = num;

	float centerX = 0.0f;
	float centerY = 0.0f;
	float size = 0.03f;
	float mass = 1.0f;
	float RV = 0.0f;

	// 정점 하나당: x,y,z,mass,vx,vy,RV = 7개 float
	// 파티클 하나당 정점 6개
	int totalFloatCount = num * 6 * 7;
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
		float vy = 0.5f + 1.5f * ((float)rand() / (float)RAND_MAX);

		float x0 = centerX - size / 2;
		float y0 = centerY - size / 2;
		float x1 = centerX + size / 2;
		float y1 = centerY + size / 2;

		RV = (float)rand() / (float)RAND_MAX;

		// v0
		particles[idx++] = x0;
		particles[idx++] = y0;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV;

		// v1
		particles[idx++] = x1;
		particles[idx++] = y0;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV;

		// v2
		particles[idx++] = x1;
		particles[idx++] = y1;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV;

		// v3
		particles[idx++] = x0;
		particles[idx++] = y0;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV;

		// v4
		particles[idx++] = x1;
		particles[idx++] = y1;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV;

		// v5
		particles[idx++] = x0;
		particles[idx++] = y1;
		particles[idx++] = 0.0f;
		particles[idx++] = mass;
		particles[idx++] = vx;
		particles[idx++] = vy;
		particles[idx++] = RV;
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
	gTime += 0.0003f;
	glUseProgram(m_TriangleShader);

	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(uTime, gTime);

	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");
	int attribRV = glGetAttribLocation(m_TriangleShader, "a_RV");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);
	glEnableVertexAttribArray(attribRV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);

	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
	glVertexAttribPointer(attribMass, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(attribVel, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (GLvoid*)(sizeof(float) * 4));
	glVertexAttribPointer(attribRV, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, 6 * m_NumParticles);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribMass);
	glDisableVertexAttribArray(attribVel);
	glDisableVertexAttribArray(attribRV);
}

void Renderer::GetGLPosition(float x, float y, float* newX, float* newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}