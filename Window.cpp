#include "Window.h"
#include <stdio.h>

void Window::CallbackHandler::error(int error, const char * message)
{
	throw message;
}

void Window::CallbackHandler::key(GLFWwindow * window, int key, int scancode, int action, int mods)
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

void Window::CallbackHandler::subscribe(GLFWwindow * window, Window * windowObj)
{
	//windows.insert(std::make_pair(window, windowObj));
	glfwSetKeyCallback(window, key);
}

void Window::CallbackHandler::unsubscribe(GLFWwindow * window)
{
	//windows.erase(windows.find(window));
	glfwSetKeyCallback(window, NULL);
}


Window::CallbackHandler::CallbackHandler()
{
	static bool first = true;
	if (first)
	{
		glfwSetErrorCallback(error);
		first = false;
		windows = {};
	}
}


Window::CallbackHandler Window::callbacks = {};
int Window::nrOfWindows = 0;


Window::Window(const char * title, size_t width, size_t height)
{
	if (nrOfWindows == 0 && !glfwInit())
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
	callbacks.subscribe(window, this);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	nrOfWindows--;
	callbacks.unsubscribe(window);

	if (nrOfWindows == 0)
		glfwTerminate();
}

void Window::use()
{
	glfwMakeContextCurrent(window);
}

void Window::clearBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::isOpen()
{
	return !glfwWindowShouldClose(window);
}

bool Window::getKey(int key)
{
	return glfwGetKey(window, key);
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

