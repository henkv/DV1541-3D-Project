#pragma once
#include <glad\glad.h>
#include "GameObjectManager.h"
#include "LightManager.h"
#include "Shader.h"
#include "Camera.h"
#include "FullscreenQuad.h"
#include "ShadowMap.h"
#include "FrontToBack.h"

class DefferedRenderer
{
private:
	FullscreenQuad fullscreenQuad;

	Shader geometryShader;
	Shader lightShader;

	ShadowMap sunShadowMap;

	GLuint geometryFramebuffer;
	GLuint positionsTexture;
	GLuint normalsTexture;
	GLuint colorsTexture;

	GLuint finalFramebuffer;
	GLuint finalTexture;

	GLuint generateVec3Texture(int width, int height);
	GLuint generateColorTexture(int width, int height);

	void createGeometryBuffers(int width, int height);
	void createFinalBuffers(int width, int height);

	void geometryPass(Camera & camera, GameObjectManager & gameObjects);
	void lightPass(Camera & camera, LightManager & lights);
	void frontToBackSort(Camera & camera, GameObjectManager & gameObjects);

public:
	DefferedRenderer(int width, int height);
	~DefferedRenderer();

	void renderScene(GameObjectManager & gameObjects, LightManager & lights, Camera & camera);
	GLuint getFinalTexture();

};

