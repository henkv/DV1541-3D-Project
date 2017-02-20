#include "DefferedRenderer.h"



GLuint DefferedRenderer::genVec3Texture(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return texture;
}

GLuint DefferedRenderer::genColorTexture(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return texture;
}

void DefferedRenderer::geometryPass(Camera & camera, GameObjectManager & gameObjects)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderGemetryPass.use();
	shaderGemetryPass.setUniform("viewMatrix", camera.getViewMatrix());
	shaderGemetryPass.setUniform("projectionMatrix", camera.getProjectionMatrix());

	gameObjects.draw(shaderGemetryPass);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DefferedRenderer::lightPass(Camera & camera, LightManager & lights)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderLightPassShader.use();
	shaderLightPassShader.setUniform("viewPosition", camera.getPosition());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePositions);
	glUniform1i(0, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureNormals);
	glUniform1i(1, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureColorSpecular);
	glUniform1i(2, 2);

	fsQuad.draw(shaderLightPassShader);
}

DefferedRenderer::DefferedRenderer(int width, int height)
	: shaderGemetryPass("shaders/GeometryPass-Vertex.glsl", "shaders/GeometryPass-Fragment.glsl")
	, shaderLightPassShader("shaders/LightPass-Vertex.glsl", "shaders/LightPass-Fragment.glsl")
{
	glGenFramebuffers(1, &gFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);

	texturePositions = genVec3Texture(width, height);
	textureNormals = genVec3Texture(width, height);
	textureColorSpecular = genColorTexture(width, height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texturePositions, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textureNormals, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, textureColorSpecular, 0);

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// - Check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw "Framebuffer incomplete";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


DefferedRenderer::~DefferedRenderer()
{
}

void DefferedRenderer::draw(Camera & camera, GameObjectManager & gameObjects, LightManager & lights)
{
	geometryPass(camera, gameObjects);
	lightPass(camera, lights);
}
