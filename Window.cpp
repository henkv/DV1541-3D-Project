#include "Window.h"

void Window::CallbackHandler::error(int error, const char * message)
{
	throw message;
}


Window::CallbackHandler::CallbackHandler()
{
	static bool first = true;
	if (first)
	{
		glfwSetErrorCallback(error);
		first = false;
	}
}


int Window::nrOfWindows = 0;


Window::Window(const char * title, size_t width, size_t height)
{
	if (nrOfWindows == 0 && !glfwInit())
		throw "failed to init glfw";
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

void Window::swapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::keyEvent(int key, int action, int mods)
{
}

void Window::pollEvents()
{
	glfwPollEvents();
}

