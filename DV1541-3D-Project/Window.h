#pragma once
#include <GLFW\glfw3.h>

class Window
{
private:
	GLFWwindow * window;

public:
	Window(const char * title, size_t width, size_t height);
	~Window();

	void use();
	bool isOpen();
	void swapBuffers();

	static void pollEvents();
};

