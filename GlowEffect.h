#pragma once
#include <glad\glad.h>
#include "Shader.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "FullscreenQuad.h"
#include "GaussianBlur.h"

class GlowEffect
{
private:
	FullscreenQuad fullscreenQuad;

	GaussianBlur gaussianBlur;

	Shader glowExtract;
	Shader mergeShader;

	GLuint glowFramebuffer;
	GLuint glowTexture;

	GLuint blurFramebuffers[2];
	GLuint blurTextures[2];

	GLuint finalFramebuffer;
	GLuint finalTexture;

	void createGlowBuffers(int width, int height);
	void createBlurBuffers(int width, int height);
	void createFinalBuffers(int width, int height);

	void renderGlowTexture(GameObjectManager & objects, Camera & camera);

public:
	GlowEffect(int width, int height);
	~GlowEffect();


	void renderGlow(GLuint sceneTexture, GameObjectManager & glowObjects, Camera & camera);
	GLuint getFinalTexture();
};

