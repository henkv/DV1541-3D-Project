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

void setBloomSamplers(Shader &shaderBloomFin);
void createhdrBuffer(GLuint &hdrFBO, GLuint colorBuffers[]);
void createBlurFBO(GLuint blurFBO[], GLuint blurColorbuffers[]);

void renderGeometry(Shader &shader, Shader &lightShader, Model &manet, GLuint &hdrFBO);
//void renderLight(Shader &lightShader, Model &manet, GLuint &hdrFBO);
void glowEffect(Shader &blurShader, Shader &shaderBloomFin, GLuint blurFBO[], GLuint colorBuffers[], GLuint blurColorbuffers[], Model &manet);

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
		Shader lightShader = { "shaders/VertexShader.glsl", "shaders/bloom_light_frag.glsl" };
		Shader blurShader = { "shaders/blur_vertex.glsl", "shaders/blur_frag.glsl" };
		Shader shaderBloomFin = { "shaders/blur_vertex.glsl", "shaders/bloom_fin_frag.glsl" };
		Model manet = { "models/manet.obj" };

		//Samplers
		setBloomSamplers(shaderBloomFin);

		//glowbuffer
		GLuint hdrFBO;  
		GLuint colorBuffers[2];
		createhdrBuffer(hdrFBO, colorBuffers);

		//blurbuffer
		GLuint blurFBO[2];
		GLuint blurColorbuffers[2];

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		while (window.isOpen())
		{
			window.pollEvents();
			//render once. Render geometry into gbuffer?
			renderGeometry(shader, lightShader, manet, hdrFBO);
			//Glow effect -> render again
			glowEffect(blurShader, shaderBloomFin, blurFBO, colorBuffers, blurColorbuffers, manet);

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

void renderFullscreenQuad()
{
	static bool firstTime = true;
	static GLuint vertexArray, arrayBuffer;

	if (firstTime)
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 uv;
		}; 

		Vertex vertices[4] = {
			{ { -1,  1, 0 },{ 0, 1 } },
			{ {  1,  1, 0 },{ 1, 1 } },
			{ { -1, -1, 0 },{ 0, 0 } },
			{ {  1, -1, 0 },{ 1, 0 } }
		};

		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &arrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));

		firstTime = false;
	}

	glBindVertexArray(vertexArray);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void setBloomSamplers(Shader &shaderBloomFin)
{
	shaderBloomFin.use();
	shaderBloomFin.setUniform("scene", 0);
	shaderBloomFin.setUniform("bloomBlur", 1);
}

void createhdrBuffer(GLuint &hdrFBO, GLuint colorBuffers[])
{
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	
	glGenTextures(2, colorBuffers);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			800,
			600,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
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
}

void createBlurFBO(GLuint blurFBO[], GLuint blurColorbuffers[])
{
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


}

void renderGeometry(Shader &shader, Shader &lightShader, Model &manet, GLuint &hdrFBO)
{
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
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
	manet.draw();

	/*lightShader.use();
	lightShader.setUniform("projection", perspective(pi<float>() * 0.2f, 8.f / 6.f, 0.1f, 100.f));
	lightShader.setUniform("view", lookAt(vec3(0, 0, -10), O, Y));
	lightShader.setUniform("world",
		rotate(
			rotate(
				rotate(I, (float)glfwGetTime(), Y),
				(float)glfwGetTime(), Z), (float)glfwGetTime(), X)
		);
	lightShader.setUniform("lightColor", vec3(1.0f, 1.0f, 1.0f));
	manet.draw();*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void glowEffect(Shader &blurShader, Shader &shaderBloomFin, GLuint blurFBO[], GLuint colorBuffers[], GLuint blurColorbuffers[], Model &manet)
{
	GLboolean horizontal = true, first_iteration = true;
	GLuint amount = 10;
	//blurrrrrr
	blurShader.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);

	for (int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[horizontal]);
		blurShader.setUniform("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : blurColorbuffers[!horizontal]);
		renderFullscreenQuad();
		horizontal = !horizontal;
		if (first_iteration)
		{
			first_iteration = false;
		}
	}

	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderBloomFin.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
	shaderBloomFin.setUniform("bloom", bloom);
	shaderBloomFin.setUniform("exposure", exposure);
	blurShader.use();
	blurShader.setUniform("image", 1);

	renderFullscreenQuad();
}