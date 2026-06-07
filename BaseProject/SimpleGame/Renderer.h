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
	void DrawTriangle_Bloom();
	void DrawFS();
	void DrawDummy();
	void DrawTexture(GLuint texID, float x, float y, float scale, bool bFlip);
	void DrawDummy_FBO();
	void DrawToFBO(GLuint fbo, void (Renderer::* drawFunc)(), GLuint tex, float x, float y, float scale, bool bFlip);
	void DrawAll_FBO();
	void DrawMultipleRenderTarget();

	void GenParticles(int num);
	GLuint CreatePngTexture(char* filePath, GLuint samplingMethod);
	void GenDummyMesh(int rx, int ry);
	void GenFBOs();
	void DrawGaussianBlur(GLuint texID, GLuint targetFBOID, GLuint shader);


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
	GLuint m_VBOTexture;

	GLuint m_SolidRectShader = 0;
	GLuint m_TriangleShader = 0;
	GLuint m_FSShader   = 0;
	GLuint m_DummyShader = 0;
	GLuint m_TextureShader;

	int m_NumParticles = 0;

	float m_RainInfo[500 * 4];

	//Texture
	GLuint m_RgbTexture = 0;
	GLuint m_NumTexture[10];
	GLuint m_NumsTexture = 0;
	GLuint m_ParticleTexture = 0;
	GLuint m_ParticleSpriteTexture = 0;
	GLuint m_PHTexture = 0;

	GLuint m_FBO = 0;
	GLuint m_FBO_Texture = 0;
	GLuint m_FBO1 = 0;
	GLuint m_FBO1_Texture = 0;
	GLuint m_FBO2 = 0;
	GLuint m_FBO2_Texture = 0;

	GLuint m_MRT_FBO = 0;
	GLuint m_MRT_FBO_Texture0 = 0;
	GLuint m_MRT_FBO_Texture1 = 0;
	GLuint m_MRT_FBO_Texture2 = 0;

	GLuint m_MRT_HDR_FBO = 0;
	GLuint m_MRT_HDR_FBO_HighTexture = 0;  //float Tex
	GLuint m_MRT_HDR_FBO_LowTexture = 0; //float Tex

	GLuint m_PingpongFBO[2];
	GLuint m_PingpongTexture[2];
	GLuint m_BlurH_Shader = 0;
	GLuint m_BlurV_Shader = 0;
};