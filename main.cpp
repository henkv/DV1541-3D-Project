#include <stdio.h>
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "FullscreenQuad.h"

int main();

struct ShadowMapReturn
{
	GLuint depthMapFBO;
	GLuint depthMap;
	GLuint SHADOW_WIDTH, SHADOW_HEIGHT;
	glm::mat4 lightSpaceMatrix;
};

void render(const Model * model);

ShadowMapReturn createShadowMap();

void initiateShaders(Shader * shader, Shader * shadowFirstPass, Shader * shadowSecondPass, ShadowMapReturn * returnValues);

void updateShadowMap(ShadowMapReturn * returnValues, Shader * shadowFirstPass, Model * manet, Model * plane);

void updateWorld(ShadowMapReturn * returnValues, Shader * shader, Shader * shadowSecondPass, Model * manet, Model * plane, Window * window);

const mat4 I;
const vec3 O = { 0, 0, 0 };
const vec3 X = { 1, 0, 0 };
const vec3 Y = { 0, 1, 0 };
const vec3 Z = { 0, 0, 1 };

const vec3 X2 = { 1, 0, 0 };
const vec3 Y2 = { 0, 1, 0 };
const vec3 Z2 = { 0, 0, 1 };
const mat4 Still = mat3(X2, Y2, Z2);

int main()
{
	try
	{
		Window window = { "DV1541 3D Project", 800, 600 };

		vec3 cameraPosition = vec3(0, 0, -10);
		vec3 cameraCenter = O;

		ShadowMapReturn returnValues = createShadowMap();

		Shader shader = { "shaders/VertexShader.glsl", "shaders/FragmentShader.glsl" };
		Shader shadowFirstPass = { "shaders/ShadowVertexShader.glsl", "shaders/ShadowFragmentShader.glsl" };
		Shader shadowSecondPass = { "shaders/WVertexShader2Pass.glsl", "shaders/WFragmentShader2Pass.glsl" };
		Shader fullScreanQuad = { "shaders/FullVertex.glsl", "shaders/FullFragment.glsl" };
		Model manet = { "models/manet.obj" };

		//Walter
		Model plane = { "models/planeBig8.obj" };
		//Walter end

		initiateShaders(&shader ,&shadowFirstPass, &shadowSecondPass, &returnValues);

		FullscreenQuad testFull;

		shader.use();
		while (window.isOpen())
		{
			window.pollEvents();
			
			//generete shadow map
			updateShadowMap(&returnValues, &shadowFirstPass, &manet, &plane);

			//render sceen
			updateWorld(&returnValues, &shader, &shadowSecondPass, &manet, &plane, &window);

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

void render(const Model * model)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model->draw();
}

ShadowMapReturn createShadowMap()
{
	ShadowMapReturn returnValues;

	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
		SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLfloat near_plane = 1.0f, far_plane = 10.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	
	returnValues.depthMap = depthMap;
	returnValues.depthMapFBO = depthMapFBO;
	returnValues.lightSpaceMatrix = lightSpaceMatrix;
	returnValues.SHADOW_HEIGHT = SHADOW_HEIGHT;
	returnValues.SHADOW_WIDTH = SHADOW_WIDTH;

	return returnValues;
}

void initiateShaders(Shader * shader, Shader * shadowFirstPass, Shader * shadowSecondPass, ShadowMapReturn * returnValues)
{
	shader->use();
	shader->setUniform("view", lookAt(vec3(0, 0, -10), O, Y));
	shader->setUniform("projection", perspective(pi<float>() * 0.2f, 8.f / 6.f, 0.1f, 100.f));
	shader->setUniform("lightSpaceMatrix", returnValues->lightSpaceMatrix);

	shadowFirstPass->use();
	shadowFirstPass->setUniform("lightSpaceMatrix", returnValues->lightSpaceMatrix);

	shadowSecondPass->use();
	shadowSecondPass->setUniform("view", lookAt(vec3(0, 0, -10), O, Y));
	shadowSecondPass->setUniform("projection", perspective(pi<float>() * 0.2f, 8.f / 6.f, 0.1f, 100.f));
	shadowSecondPass->setUniform("lightSpaceMatrix", returnValues->lightSpaceMatrix);
}

void updateShadowMap(ShadowMapReturn * returnValues, Shader * shadowFirstPass, Model * manet, Model * plane)
{

	shadowFirstPass->use();
	shadowFirstPass->setUniform("model", Still);

	glCullFace(GL_FRONT);

	glViewport(0, 0, returnValues->SHADOW_WIDTH, returnValues->SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, returnValues->depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	manet->draw();
	plane->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void updateWorld(ShadowMapReturn * returnValues, Shader * shader, Shader * shadowSecondPass, Model * manet, Model * plane, Window * window)
{
	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->use();
	shader->setUniform("globalTime", Window::getTime());
	shader->setUniform("world", Still);
	//	rotate(
	//		rotate(
	//			rotate(I, (float)glfwGetTime(), Y),
	//			(float)glfwGetTime(), Z),
	//			(float)glfwGetTime(), X)
	//);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, returnValues->depthMap);
	glUniform1i(4, 0);
	manet->draw();//RenderScene();
	shadowSecondPass->use();
	shadowSecondPass->setUniform("stillWorld", Still);
	plane->draw();

	//test of depth map
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//fullScreanQuad.use();
	//testFull.draw(fullScreanQuad);
	//walter end

	window->swapBuffers();
}