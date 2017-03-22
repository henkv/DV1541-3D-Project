#pragma once
#include "Shader.h"
#include "FullscreenQuad.h"
#include <vector>
#include "glm\glm.hpp"
#include "Camera.h"
#include <string>
#include "GaussianBlur.h"

class SSAO
{
private:
	FullscreenQuad fsQuad;
	GaussianBlur blur;

	Shader ssaoShader;

	GLuint framebuffer;
	GLuint ssaoMap;


	std::vector<glm::vec3> randomPoints;

	float random();

	void createBuffers(int width, int height);

public:
	SSAO(int width, int height);
	~SSAO(); 

	void render(Camera & camera, GLuint viewPositionMap);
	GLuint getSsaoMap();
};

