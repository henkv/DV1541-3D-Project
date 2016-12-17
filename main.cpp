#include <stdio.h>
#include <glad\glad.h>

#include "Window.h"
#include "Shader.h"
#include "Model.h"

void render(const Model * model);


int main()
{

	try
	{
		Window window = { "DV1541 3D Project", 800, 600 };
		Shader shader = { "VertexShader.glsl", "FragmentShader.glsl" };
		Model cube = { "cube.obj" };

		shader.use();
		while (window.isOpen())
		{
			window.pollEvents();

			render(&cube);

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

void render(const Model * model)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model->draw();
}
