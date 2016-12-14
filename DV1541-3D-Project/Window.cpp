#include "Window.h"


Window::Window(const char * title, size_t width, size_t height)
{
	if (!glfwInit())
		throw "failed to init glfw";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
		throw "failed to create window";
}

Window::~Window()
{
}

void Window::use()
{
	glfwMakeContextCurrent(window);
}

bool Window::isOpen()
{
	bool shouldClose = glfwWindowShouldClose(window);

	if (shouldClose)
	{
		glfwDestroyWindow(window);
	}

	return !shouldClose;
}

void Window::swapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::pollEvents()
{
	glfwPollEvents();
}
