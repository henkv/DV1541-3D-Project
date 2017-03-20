#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include "Shader.h"
#include "Vertex.h"


using namespace glm;

class FullscreenQuad
{
private:
	Shader textureShader;

	GLuint vertexArray;
	GLuint arrayBuffer;

public:
	FullscreenQuad();
	~FullscreenQuad();

	void draw();
	void drawTexture(GLuint texture);
};

