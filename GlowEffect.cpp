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
	glGenBuffers(2, blurFramebuffers);
	glGenTextures(2, blurTextures);

	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurFramebuffers[i]);

		glBindTexture(GL_TEXTURE_2D, blurTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTextures[i], 0);

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


GlowEffect::GlowEffect(int width, int height)
	: glowExtract("shaders/GlowExtract.vert", "shaders/GlowExtract.frag")
	, blurEffect("shaders/FullscreenQuad.vert", "shaders/BlurEffect.frag")
	, mergeShader("shaders/FullscreenQuad.vert", "shaders/GlowMerge.frag")
{
	createGlowBuffers(width, height);
	createBlurBuffers(width, height);
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
	bool horizontalPass = true;
	static int passes = 10;

	blurEffect.use();

	glBindFramebuffer(GL_FRAMEBUFFER, blurFramebuffers[0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	blurEffect.setUniform("horizontalPass", 0);
	blurEffect.setTexture2D(0, "glow", glowTexture);
	fullscreenQuad.draw();
	
	//for (int i = 0; i < passes; i++)
	//{
		horizontalPass = !horizontalPass;

		glBindFramebuffer(GL_FRAMEBUFFER, blurFramebuffers[1]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		blurEffect.setUniform("horizontalPass", 0);
		blurEffect.setTexture2D(0, "glow", blurTextures[0]);
		fullscreenQuad.draw();
	//}

	glBindFramebuffer(GL_FRAMEBUFFER, glowFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	blurEffect.setUniform("horizontalPass", 0);
	blurEffect.setTexture2D(0, "glow", blurTextures[1]);
	fullscreenQuad.draw();
}

void GlowEffect::renderGlow(GLuint sceneTexture, GameObjectManager & glowObjects, Camera & camera)
{
	renderGlowTexture(glowObjects, camera);
	blurGlowTexutre();

	glBindFramebuffer(GL_FRAMEBUFFER, glowFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mergeShader.use();
	mergeShader.setTexture2D(0, "scene", sceneTexture);
	mergeShader.setTexture2D(1, "glow", glowTexture);

	fullscreenQuad.draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GlowEffect::getGlowTexture()
{
	return glowTexture;
}
