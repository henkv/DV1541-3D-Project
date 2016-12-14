#include <GL\glew.h>

#include "Window.h"

int main()
{
	Window window = { "DV1541 3D Project", 800, 600 };

	window.use();
	glewInit();

	while (window.isOpen())
	{
		window.pollEvents();


		window.swapBuffers();
	}

	return 0;
}