#pragma once
#include <glad\glad.h>
#include "GameObjectManager.h"
#include "LightManager.h"
#include "Shader.h"
#include "Camera.h"
#include "FullscreenQuad.h"

class DefferedRenderer
{
private:
	GLuint gFrameBuffer;
	GLuint texturePositions;
	GLuint textureNormals;
	GLuint textureColorSpecular;

	Shader shaderGemetryPass;
	Shader shaderLightPassShader;

	GLuint genVec3Texture(int width, int height);
	GLuint genColorTexture(int width, int height);

	FullscreenQuad fsQuad;

	void geometryPass(Camera & camera, GameObjectManager & gameObjects);
	void lightPass(Camera & camera, LightManager & lights);

public:
	DefferedRenderer(int width, int height);
	~DefferedRenderer();

	void draw(Camera & camera, GameObjectManager & gameObjects, LightManager & lights);

};

