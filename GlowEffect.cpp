#include "GlowEffect.h"



void GlowEffect::createGlowBuffers(int width, int height)
{
	glGenFramebuffers(1, &glowFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, glowFramebuffer);

	glGenTextures(1, &glowTexture);
	glBindTexture(GL_TEXTURE_2D, glowTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glowTexture, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw "Glow Effect: Glow Framebuffer not complete!";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlowEffect::createBlurBuffers(int width, int height)
{
	glGenFramebuffers(2, blurFramebuffers);
	glGenTextures(2, blurTextures);

	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurFramebuffers[i]);
		glBindTexture(GL_TEXTURE_2D, blurTextures[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTextures[i], 0);

		GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);

		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw "Glow Effect: Blur Framebuffer not complete!";
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlowEffect::createFinalBuffers(int width, int height)
{
	glGenFramebuffers(1, &finalFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, finalFramebuffer);

	glGenTextures(1, &finalTexture);
	glBindTexture(GL_TEXTURE_2D, finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalTexture, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw "Glow Effect: Glow Framebuffer not complete!";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


GlowEffect::GlowEffect(int width, int height)
	: glowExtract("shaders/GlowExtract.vert", "shaders/GlowExtract.frag")
	, blurEffectH("shaders/FullscreenQuad.vert", "shaders/BlurEffectHorizontal.frag")
	, blurEffectV("shaders/FullscreenQuad.vert", "shaders/BlurEffectVertical.frag")
	, mergeShader("shaders/FullscreenQuad.vert", "shaders/GlowMerge.frag")
{
	createGlowBuffers(width, height);
	createBlurBuffers(width, height);
	createFinalBuffers(width, height);
}

GlowEffect::~GlowEffect()
{
}



void GlowEffect::renderGlowTexture(GameObjectManager & objects, Camera & camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, glowFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glowExtract.use();
	glowExtract.setUniform("viewMatrix", camera.getViewMatrix());
	glowExtract.setUniform("projectionMatrix", camera.getProjectionMatrix());

	objects.draw(glowExtract);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlowEffect::blurGlowTexutre()
{
	static int passes = 100;
	bool horizontalPass = true;
	Shader * blurEffect = &blurEffectH;

	for (int i = 1; i <= passes; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, i == passes ? glowFramebuffer : blurFramebuffers[horizontalPass]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		blurEffect->use();
		blurEffect->setTexture2D(0, "glow", i == 1 ? glowTexture : blurTextures[!horizontalPass]);
		fullscreenQuad.draw();

		horizontalPass = !horizontalPass;
		blurEffect = &(horizontalPass ? blurEffectH : blurEffectV);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GlowEffect::renderGlow(GLuint sceneTexture, GameObjectManager & glowObjects, Camera & camera)
{
	renderGlowTexture(glowObjects, camera);
	blurGlowTexutre();

	glBindFramebuffer(GL_FRAMEBUFFER, finalFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mergeShader.use();
	mergeShader.setTexture2D(0, "scene", sceneTexture);
	mergeShader.setTexture2D(1, "glow", glowTexture);

	fullscreenQuad.draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GlowEffect::getFinalTexture()
{
	return finalTexture;
}
