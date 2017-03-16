#include "ShadowMap.h"
#include <glm\gtc\matrix_transform.hpp>


ShadowMap::ShadowMap(int windowWidth, int windowHeight)
	: shadowMapShader("shaders/ShadowMap.vert", "shaders/ShadowMap.frag")
	, WINDOW_WIDTH(windowWidth)
	, WINDOW_HEIGHT(windowHeight)
{
	createDepthMapBuffers();


	lightPosition = vec3(20.0f, 20.0f, 0.0f);
	lightDirection = normalize(vec3(0, 0, 0) - lightPosition);


	GLfloat nearPlane = 1.0f;
	GLfloat farPlane = 50.0f;
	mat4 lightProjection(ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane));
	mat4 lightView(lookAt(lightPosition, lightPosition + lightDirection, vec3(0.0f, 1.0f, 0.0f)));

	lightSpaceMatrix = lightProjection * lightView;
}


ShadowMap::~ShadowMap()
{
}

void ShadowMap::renderShadowMap(GameObjectManager & objects)
{
	glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFramebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadowMapShader.use();
	shadowMapShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);
	objects.draw(shadowMapShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

GLuint ShadowMap::getShadowMapTexture()
{
	return depthMap;
}

vec3 ShadowMap::getLightPosition()
{
	return lightPosition;
}

vec3 ShadowMap::getLightDirection()
{
	return lightDirection;
}

mat4 ShadowMap::getLightSpaceMatrix()
{
	return lightSpaceMatrix;
}


void ShadowMap::createDepthMapBuffers()
{
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &depthMapFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFramebuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
