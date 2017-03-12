#pragma once
#include <glad\glad.h>
#include "Shader.h"

class GlowEffect
{
private:
	Shader glowExtract;
	Shader blurEffect;
	Shader mergeShader;

	GLuint glowFramebuffer;
	GLuint glowTexture;

	enum BlurPass { HORIZONTAL, VERTICAL };
	GLuint blurFramebuffers[2];
	GLuint blurTextures[2];
	BlurPass blurPass;

	void createGlowBuffers(int width, int height);
	void createBlurBuffers(int width, int height);

public:
	GlowEffect();
	~GlowEffect();

	void renderGlowTexture()
};

