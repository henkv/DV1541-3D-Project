#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include "Shader.h"

using namespace glm;

class FullscreenQuad
{
private:
	Shader textureShader;

	struct Vertex
	{
		vec3 position;
		vec2 uv;
	};

	GLuint vertexArray;
	GLuint arrayBuffer;

public:
	FullscreenQuad();
	~FullscreenQuad();

	void draw();
	void drawTexture(GLuint texture);
};

