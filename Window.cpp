#include "Window.h"
#include <stdio.h>

void Window::mouseButton(GLFWwindow * window, int button, int action, int mods)
{
	Input::setMouseButton((Input::Mouse)button, action == GLFW_PRESS);
}

void Window::mouseMove(GLFWwindow * window, double xpos, double ypos)
{
	int height, width;
	glfwGetWindowSize(window, &height, &width);

	float x = (xpos / (float)width) * 2.0 - 1;
	float y = (ypos / (float)height) * 2.0 - 1;

	Input::setMousePos(x, y);
}

void Window::error(int error, const char * message)
{
	throw message;
}

void Window::key(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key != -1)
	{
		if (action == GLFW_PRESS)
		{
			Input::setKey((Input::Key)key, true);
		}
		else if (action == GLFW_RELEASE)
		{
			Input::setKey((Input::Key)key, false);
		}
	}
}

int Window::nrOfWindows = 0;

Window::Window(const char * title, size_t width, size_t height)
{
	static void *e = glfwSetErrorCallback(error);

	if (!glfwInit())
		throw "failed to init glfw";
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window)
		throw "failed to create window";

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

	nrOfWindows++;
	glfwSetKeyCallback(window, key);
	glfwSetCursorPosCallback(window, mouseMove);
	glfwSetMouseButtonCallback(window, mouseButton);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	nrOfWindows--;

	if (nrOfWindows == 0)
		glfwTerminate();
}

void Window::use()
{
	glfwMakeContextCurrent(window);
}

bool Window::isOpen()
{
	return !glfwWindowShouldClose(window);
}

void Window::swapBuffer()
{
	glfwSwapBuffers(window);
}

double Window::getTime()
{
	return glfwGetTime();
}

void Window::pollEvents()
{
	glfwPollEvents();
}

