
#include <stdio.h>
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "LightManager.h"
#include "FullscreenQuad.h"

const size_t WINDOW_WIDTH = 800;
const size_t WINDOW_HEIGHT = 600;

struct GBuffer
{
	GLuint gBuffer, gPosition, gNormal, gColorSpec;
};
int main();
GBuffer DefferedRenderingSetup();
void GeometryPass(GLuint gBuffer, Shader & geometryPassShader, GameObjectManager & objectManager, Camera & camera);
void LightningPass(GBuffer gBuffer, Shader & shader, LightManager & lights, Camera & camera, FullscreenQuad & quad);


int main()
{

	try
	{
		Window window = { "DV1541 3D Project", WINDOW_WIDTH, WINDOW_HEIGHT };
		Camera camera = { {0,0,-10}, {0,0,1}, {0,1,0} };
		FullscreenQuad quad;

		Shader phongShader("shaders/Phong-Vertex.glsl", "shaders/Phong-Fragment.glsl");
		Shader geometryPassShader("shaders/GeometryPass-Vertex.glsl", "shaders/GeometryPass-Fragment.glsl");
		Shader lightPassShader("shaders/LightPass-Vertex.glsl", "shaders/LightPass-Fragment.glsl");

		GBuffer gBuffer = DefferedRenderingSetup();
		GameObjectManager objectManager;
		LightManager lightManager;

		Model manet = { "models/manet.obj" };

		objectManager.add(&manet);


		double thisFrame = 0, lastFrame = 0;
		while (window.isOpen())
		{
			thisFrame = window.getTime();
			window.pollEvents();


			GeometryPass(gBuffer.gBuffer, geometryPassShader, objectManager, camera);
			LightningPass(gBuffer, lightPassShader, lightManager, camera, quad);


			window.swapBuffer();
			window.clearBuffer();
			lastFrame = thisFrame;
		}
	}
	catch (const char * message)
	{
		printf("Error: %s", message);
		printf("\nPress enter to exit...");
		getchar();
	}
	
	return 0;
}


GBuffer DefferedRenderingSetup()
{
	GLuint gBuffer, gPosition, gNormal, gColorSpec;

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// - Position Color Buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// - Normal Color Buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// - Color + Specular Color Buffer
	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

	// - Tell OpenGL which color attachemnts to use for this framebuffer
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// - Create and attach depth buffer
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	// - Check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw "Framebuffer incomplete";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return { gBuffer, gPosition, gNormal, gColorSpec };
}

void GeometryPass(GLuint gBuffer, Shader & shader, GameObjectManager& objectManager, Camera & camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	Shader::setUniform("viewMatrix", camera.getViewMatrix());
	Shader::setUniform("projectionMatrix", camera.getProjectionMatrix());

	objectManager.draw(shader);
}

void LightningPass(GBuffer gBuffer, Shader& shader, LightManager & lights, Camera & camera, FullscreenQuad & quad)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	shader.use();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
	glUniform1i(0, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
	glUniform1i(1, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer.gColorSpec);
	glUniform1i(2, 2);

	//lights.sendTo(shader);
	shader.setUniform("viewPosition", camera.getPosition());
	quad.draw(shader);
}