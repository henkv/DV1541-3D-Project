#pragma once
#include <glad\glad.h>
#include "Shader.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "FullscreenQuad.h"

class GlowEffect
{
private:
	FullscreenQuad fullscreenQuad;

	Shader glowExtract;
	Shader blurEffect;
	Shader mergeShader;

	GLuint glowFramebuffer;
	GLuint glowTexture;

	GLuint blurFramebuffers[2];
	GLuint blurTextures[2];

	void createGlowBuffers(int width, int height);
	void createBlurBuffers(int width, int height);

	void renderGlowTexture(GameObjectManager & objects, Camera & camera);
	void blurGlowTexutre();

public:
	GlowEffect(int width, int height);
	~GlowEffect();


	void renderGlow(GLuint sceneTexture, GameObjectManager & glowObjects, Camera & camera);
	GLuint getGlowTexture();
};

