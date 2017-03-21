#pragma once
#include <glad\glad.h>
#include "Shader.h"
#include "FullscreenQuad.h"

class GaussianBlur
{
private:
	FullscreenQuad quad;

	Shader blurShader;
	GLuint pingPongFramebuffer[3];
	GLuint pingPongColorbuffer[3];

	void createPingPongBuffers(int width, int height);

public:
	GaussianBlur(int width, int height);
	~GaussianBlur();

	void blur(GLuint texture, int passes);
	GLuint getBlurTexture();
};

