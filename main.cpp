#include <stdio.h>
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Window.h"
#include "Shader.h"
#include "Model.h"

int main();

void render(const Model * model);
void update(double deltaTime);

const mat4 I;
const vec3 O = { 0, 0, 0 };
const vec3 X = { 1, 0, 0 };
const vec3 Y = { 0, 1, 0 };
const vec3 Z = { 0, 0, 1 };

Window window = { "DV1541 3D Project", 800, 600 };

struct {
	vec3 position = { 0, 0, -10 };
	vec3 direction = { 0, 0, 1 };
} camera;

int main()
{
	try
	{
		Shader shader = { "shaders/VertexShader.glsl", "shaders/FragmentShader.glsl" };
		Model manet = { "models/manet.obj" };

		shader.use();
		shader.setUniform("view", lookAt(camera.position, camera.position + camera.direction, Y));
		shader.setUniform("projection", perspective(pi<float>() * 0.2f, 8.f / 6.f, 0.1f, 100.f));
		shader.setUniform("viewPoint", camera.position);

		double lastFrame = window.getTime();
		while (window.isOpen())
		{
			window.pollEvents();
			update(window.getTime() - lastFrame);
			lastFrame = window.getTime();

			shader.setUniform("view", lookAt(camera.position, camera.position + camera.direction, Y));
			shader.setUniform("globalTime", Window::getTime());
			shader.setUniform("world", 
				rotate(
					rotate(
					rotate(I, (float)glfwGetTime(), Y),
					(float)glfwGetTime(), Z), (float)glfwGetTime(), X)
			);

			render(&manet);

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

void update(double deltaTime)
{
	bool W = window.getKey(GLFW_KEY_W);
	bool S = window.getKey(GLFW_KEY_S);
	bool A = window.getKey(GLFW_KEY_A);
	bool D = window.getKey(GLFW_KEY_D);
	
	if (W)
	{
		camera.position.z += deltaTime ;
	}
	else if (S)
	{
		camera.position.z -= deltaTime;
	}

	if (A && !D)
	{
		camera.position.x += deltaTime;
	}

	if (D && !A)
	{
		camera.position.x -= deltaTime;
	}
}

void render(const Model * model)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model->draw();
}
