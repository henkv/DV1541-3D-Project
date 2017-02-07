#include "main.h"
#include <stdio.h>
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "GameObjectManager.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main()
{

	try
	{
		Window window = { "DV1541 3D Project", 800, 600 };
		Camera camera = { vec3(0,0,-10), vec3(0,0,1), vec3(0,1,0) };
		Shader geometryPassShader("shaders/GeometryPass-Vertex.glsl", "shaders/GemetryPass-Fragment.glsl");
		Shader lightningPassShader("shaders/LightningPass-Vertex.glsl", "shaders/LightningPass-Fragment.glsl");
		Model manet = { "models/manet.obj" };

		GBuffer gBuffer = DefferedRenderingSetup();
		GameObjectManager objectManager;
		objectManager.add(&manet);


		double lastFrame = window.getTime();
		double thisFrame = window.getTime();
		while (window.isOpen())
		{
			window.pollEvents();

			GeometryPass(gBuffer.gBuffer, geometryPassShader, objectManager, camera);
			LightningPass(gBuffer, lightningPassShader, lightManager, camera);

			window.swapBuffers();
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

	return { gBuffer, gPosition, gNormal, gColorSpec };
}

void GeometryPass(GLuint gBuffer, Shader & shader, GameObjectManager& objectManager, Camera & camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	shader.setUniform("viewMatrix", camera.getViewMatrix());
	shader.setUniform("projectionMatrix", camera.getProjectionMatrix());

	objectManager.draw(shader);
}

void LightningPass(GBuffer gBuffer, Shader& shader, LightManager & lights, Camera & camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	shader.use();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer.gColorSpec);

	lights.sendTo(shader);
	shader.setUniform("viewPosition", camera.getPosition());
}