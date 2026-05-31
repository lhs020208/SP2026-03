//--------------------------------------------------------
//Renderer.h
//--------------------------------------------------------
#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void DrawTriangle();
	void DrawFS();
	void DrawDummy();
	void GenParticles(int num);
	GLuint CreatePngTexture(char* filePath, GLuint samplingMethod);
	void GenDummyMesh(int rx, int ry);

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string* target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float* newX, float* newY);

	bool m_Initialized = false;

	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBOTriangle = 0;
	GLuint m_VBOFS = 0;

	GLuint m_VBO_Dummy = 0;
	GLuint m_VBO_DummyCount = 0;

	GLuint m_SolidRectShader = 0;
	GLuint m_TriangleShader = 0;
	GLuint m_FSShader   = 0;
	GLuint m_DummyShader = 0;

	int m_NumParticles = 0;

	float m_RainInfo[500 * 4];

	//Texture
	GLuint m_RgbTexture = 0;
	GLuint m_NumTexture[10];
	GLuint m_NumsTexture = 0;
	GLuint m_ParticleTexture = 0;
	GLuint m_ParticleSpriteTexture = 0;
	GLuint m_PHTexture = 0;
};