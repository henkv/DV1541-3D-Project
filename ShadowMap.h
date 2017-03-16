#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
using namespace glm;

#include "Shader.h"
#include "GameObjectManager.h"

class ShadowMap
{
private:
	const int SHADOWMAP_WIDTH = 1024;
	const int SHADOWMAP_HEIGHT = 1024;

	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;

	Shader shadowMapShader;

	GLuint depthMapFramebuffer;
	GLuint depthMap;

	vec3 lightPosition;
	vec3 lightDirection;
	mat4 lightSpaceMatrix;

	void createDepthMapBuffers();

public:
	ShadowMap(int windowWidth, int windowHeight);
	~ShadowMap();

	void renderShadowMap(GameObjectManager & objects);
	GLuint getShadowMapTexture();

	vec3 getLightPosition();
	vec3 getLightDirection();
	mat4 getLightSpaceMatrix();
};

