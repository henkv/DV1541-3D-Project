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
#include "Input.h"
#include "DefferedRenderer.h"

const size_t WINDOW_WIDTH = 800;
const size_t WINDOW_HEIGHT = 600;

const mat4 I;
const vec3 O = { 0, 0, 0 };
const vec3 X = { 1, 0, 0 };
const vec3 Y = { 0, 1, 0 };
const vec3 Z = { 0, 0, 1 };

GLboolean bloom = true;
GLfloat exposure = 1.0f;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		Window window = { "DV1541 3D Project", WINDOW_WIDTH, WINDOW_HEIGHT };
		Camera camera = { {0, 0, -5}, {0,0,0}, {0,1,0} };
		

		DefferedRenderer defferedRenderer(WINDOW_WIDTH, WINDOW_HEIGHT);

		Shader phongShader("shaders/Phong.vert", "shaders/Phong.frag");

		GameObjectManager objectManager;
		LightManager lightManager;
		lightManager.add(LightManager::Light( vec3(-10,  0, -10), vec3(1, 0, 0) ));
		lightManager.add(LightManager::Light( vec3( 10, -10, -10), vec3(.75, 1, 0) ));
		lightManager.add(LightManager::Light( vec3(  0, 10,   0), vec3(1, 1, 1) ));

		Window window = { "DV1541 3D Project", 800, 600 };
		Shader shader = { "shaders/VertexShader.glsl", "shaders/BackfaceCulling.glsl", "shaders/FragmentShader.glsl" };
		Shader blurShader = { "shaders/blur_vertex.glsl", "shaders/blur_frag.glsl" };
		Shader shaderBloomFin = { "shaders/blur_vertex.glsl", "shaders/bloom_fin_frag.glsl" };
		Model manet = { "models/manet.obj" };

		//Samplers
		shaderBloomFin.use();
		glUniform1i(glGetUniformLocation(shaderBloomFin, "scene"), 0);
		glUniform1i(glGetUniformLocation(shaderBloomFin, "bloomBlur"), 1);

		//Olika objekt? Är objekten de olika "stegen"? 
		//En för texturPos, Normaler, Färg, Ljus...
		//Behövs en helt egen för glow? Eller räcker det med att ta en vi redan har?
		//Hur många saker får plats i en gBuffer? Behöver man flera? Vilka har vi? Vilka behövs inte?
		//Är detta en korrekt buffer? Eller har vi redan en sådan? Behöver jag ändra parametrar? VAD SKA JAG HA I gBUFFERN?
		//Hur blir det med Shaders? Hur många shaders kommer jag behöva skriva enbart för glow? Minst två för varje objekt? Då den nuvarande har två men vi behöver fler för en glow
		//och en blur effekt. Kan de ligga i samma? Behöver ljusen en egen shader? Finns det? Måste det skrivas? Vart implementerar jag resten av gBuffer? Game-loop?
		//Kombinera sakerna i buffern? Multiplicera eller sker det automatiskt? Hur gör jag isåfall det? Kommando/funktion?


		//Jag har bara ett enda objekt på skärmen och hela det objektet ska ha glow effekt. 
		//1. Rendera normalt.
		//2. Rendera samma modell med glow effect ovanpå den första rendreringen.

		//Glow sker med Gaussian-blur. (Blurra horizontalt och sedan verticalt, two-pass) 
		//Hur görs detta? Hur gör man två-pass saker?
		//I vilken shader ska gaussian blur implementeras? Vad behöver gaussian-blur för att kunna köras, parametrar osv.

		//glowbuffer
		GLuint hdrFBO;
		glGenFramebuffers(1, &hdrFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		GLuint colorBuffers[2];
		glGenTextures(2, colorBuffers);
		for (int i = 0; i < 2; i++)
		{
			glBindTexture (GL_TEXTURE_2D, colorBuffers[i]);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGB16F,
				800,
				600,
				0,
				GL_RGB,
				GL_FLOAT,
				NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
		}

		GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("Framebuffer not complete!");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//blurbuffer
		GLuint blurFBO[2];
		GLuint blurColorbuffers[2];
		glGenFramebuffers(2, blurFBO);
		glGenTextures(2, blurColorbuffers);
		for (int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
			glBindTexture(GL_TEXTURE_2D, blurColorbuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColorbuffers[i], 0);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				printf("Framebuffer not complete!");
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//shader.use();
		/*shader.setUniform("view", lookAt(vec3(0, 0, -10), O, Y));
		shader.setUniform("projection", perspective(pi<float>() * 0.2f, 8.f / 6.f, 0.1f, 100.f));
		shader.setUniform("viewPoint", vec3(0, 0, -10));
		objectManager.add(&manet);


		double thisFrame = 0, lastFrame = 0, deltaTime = 0;
		while (window.isOpen())
		{
			thisFrame = window.getTime();
			deltaTime = thisFrame - lastFrame;
			window.pollEvents();
			//render once. Render geometry into gbuffer?
			glBindFramebuffer (GL_FRAMEBUFFER, hdrFBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.use();
			shader.setUniform("view", lookAt(vec3(0, 0, -10), O, Y));
			shader.setUniform("projection", perspective(pi<float>() * 0.2f, 8.f / 6.f, 0.1f, 100.f));
			shader.setUniform("viewPoint", vec3(0, 0, -10));
			shader.setUniform("globalTime", (float)glfwGetTime());
			shader.setUniform("world", 
				rotate(
					rotate(
					rotate(I, (float)glfwGetTime(), Y),
					(float)glfwGetTime(), Z), (float)glfwGetTime(), X)
			);
			render(&manet);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//Glow effect -> render again
			GLboolean horizontal = true, first_iteration = true;
			GLuint amount = 10;
			//blurrrrrr
			blurShader.use();
			for (int i = 0; i < amount; i++)
			{
				glBindFramebuffer (GL_FRAMEBUFFER, blurFBO[horizontal]);
				glUniform1i(glGetUniformLocation(blurShader, "horizontal"), horizontal);
				glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : blurColorbuffers[!horizontal]);
				render(&manet);
				horizontal = !horizontal;
				if (first_iteration)
				{
					first_iteration = false;
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//???
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shaderBloomFin.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, blurColorbuffers[!horizontal]);
			glUniform1i(glGetUniformLocation(shaderBloomFin, "bloom"), bloom);
			glUniform1i(glGetUniformLocation(shaderBloomFin, "exposure"), exposure);
			render(&manet);

			

			camera.update(deltaTime);
			defferedRenderer.draw(camera, objectManager, lightManager);

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

	//Delete the buffer

	return 0;
}
