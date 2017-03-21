#include "GaussianBlur.h"



void GaussianBlur::createPingPongBuffers(int width, int height)
{
	glGenFramebuffers(3, pingPongFramebuffer);
	glGenTextures(3, pingPongColorbuffer);

	for (int i = 0; i < 3; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingPongFramebuffer[i]);
		glBindTexture(GL_TEXTURE_2D, pingPongColorbuffer[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingPongColorbuffer[i], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw "GaussianBlur: Framebuffer not complete!";
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
		
GaussianBlur::GaussianBlur(int width, int height)
	: blurShader("shaders/GaussianBlur.vert", "shaders/GaussianBlur.frag")
{
	createPingPongBuffers(width, height);
}

GaussianBlur::~GaussianBlur()
{
}

void GaussianBlur::blur(GLuint texture, int passes)
{
	bool horizontalPass = true;

	blurShader.use();
	for (int i = 1; i <= passes; i++)
	{
		horizontalPass = !horizontalPass;
		glBindFramebuffer(GL_FRAMEBUFFER, pingPongFramebuffer[i == passes ? 2 : horizontalPass]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		blurShader.setUniform("horizontalPass", horizontalPass);
		blurShader.setTexture2D(0, "image", i == 1 ? texture : pingPongColorbuffer[!horizontalPass]);
		quad.draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GaussianBlur::getBlurTexture()
{
	return pingPongColorbuffer[2];
}