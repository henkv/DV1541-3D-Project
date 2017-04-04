#include "DefferedRenderer.h"


DefferedRenderer::DefferedRenderer(int width, int height)
	: geometryShader("shaders/GeometryPass.vert", "shaders/GeometryPass.frag")
	, geometryShaderTesselated( "shaders/GeometryPass-Tesselated.vert"
							  , "shaders/GeometryPass-Tesselated.tesc"
							  , "shaders/GeometryPass-Tesselated.tese"
							  , "shaders/GeometryPass-Tesselated.frag")
	, lightShader("shaders/LightPass.vert", "shaders/LightPass.frag")
	, sunShadowMap(width, height)
	, ssao(width, height)
{
	createGeometryBuffers(width, height);
	createFinalBuffers(width, height);
}


DefferedRenderer::~DefferedRenderer()
{
}



GLuint DefferedRenderer::generateVec3Texture(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return texture;
}

GLuint DefferedRenderer::generateColorTexture(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void DefferedRenderer::createGeometryBuffers(int width, int height)
{
	glGenFramebuffers(1, &geometryFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, geometryFramebuffer);

	positionsTexture = generateVec3Texture(width, height);
	normalsTexture = generateVec3Texture(width, height);
	colorsTexture = generateColorTexture(width, height);
	viewPositionsTexture = generateVec3Texture(width, height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionsTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalsTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorsTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, viewPositionsTexture, 0);

	GLuint attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw "DefferedRender: Geometry Framebuffer incomplete";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DefferedRenderer::createFinalBuffers(int width, int height)
{
	glGenFramebuffers(1, &finalFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, finalFramebuffer);

	finalTexture = generateColorTexture(width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw "DefferedRender: Final Framebuffer incomplete";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DefferedRenderer::geometryPass(Camera & camera, GameObjectManager & gameObjects)
{
	sunShadowMap.renderShadowMap(gameObjects);

	glBindFramebuffer(GL_FRAMEBUFFER, geometryFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	geometryShader.use();
	geometryShader.setUniform("eyePosition", camera.getPosition());
	geometryShader.setUniform("viewMatrix", camera.getViewMatrix());
	geometryShader.setUniform("projectionMatrix", camera.getProjectionMatrix());

	gameObjects.draw(geometryShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DefferedRenderer::lightPass(Camera & camera, LightManager & lights)
{
	ssao.render(camera, viewPositionsTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, finalFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightShader.use();
	lightShader.setUniform("viewPosition", camera.getPosition());

	lightShader.setTexture2D(0, "Positions", positionsTexture);
	lightShader.setTexture2D(1, "Normals", normalsTexture);
	lightShader.setTexture2D(2, "ColorSpecs", colorsTexture);
	lightShader.setTexture2D(3, "SSAOMap", ssao.getSsaoMap());
	lightShader.setTexture2D(4, "sunShadowMap", sunShadowMap.getShadowMapTexture());

	lightShader.setUniform("sunColor", vec3(1.0, 1.0, 1.0));
	lightShader.setUniform("sunDirection", sunShadowMap.getLightDirection());
	lightShader.setUniform("sunLightSpaceMatrix", sunShadowMap.getLightSpaceMatrix());

	fullscreenQuad.draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DefferedRenderer::renderScene(GameObjectManager & gameObjects, LightManager & lights, Camera & camera)
{
	geometryPass(camera, gameObjects);
	lightPass(camera, lights);
}

GLuint DefferedRenderer::getFinalTexture()
{
	return finalTexture;
}
