#include "SSAO.h"


float SSAO::random()
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return r * 2.0f - 1.0f;
}

void SSAO::createBuffers(int width, int height)
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &ssaoMap);
	glBindTexture(GL_TEXTURE_2D, ssaoMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoMap, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw "SSAO: Glow Framebuffer not complete!";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



SSAO::SSAO(int width, int height)
	:ssaoShader("shaders/SSAO.vert", "shaders/SSAO.frag")
{
	createBuffers(width, height);

	ssaoShader.use();
	ssaoShader.setUniform("nrOfRandomPoints", 32);
	for (int i = 0; i < 32; i++)
	{
		ssaoShader.setUniform(("randomPoints[" + std::to_string(i) + "]").c_str(),
			normalize(vec3(random(), random(), random())) * 0.1f
		);
	}
}

SSAO::~SSAO()
{
}

void SSAO::render(Camera & camera, GLuint viewPositionMap)
{
	ssaoShader.use();
	ssaoShader.setTexture2D(0, "viewPositions", viewPositionMap);
	ssaoShader.setUniform("projectionMatrix", camera.getProjectionMatrix());


	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fsQuad.draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint SSAO::getSsaoMap()
{
	return ssaoMap;
}
