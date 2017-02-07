#pragma once
struct GBuffer
{
	GLuint gBuffer, gPosition, gNormal, gColorSpec;
};
GBuffer DefferedRenderingSetup();

void GeometryPass(GLuint gBuffer, Shader & geometryPassShader, GameObjectManager & objectManager, Camera & camera);

void LightningPass(GBuffer gBuffer, Shader & shader, LightManager & lights, Camera & camera);
