#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>


class Window
{
private:
	static class CallbackHandler
	{
	public:
		static void error(int error, const char * message);
	
		CallbackHandler();
	} callbacks;

	static int nrOfWindows;
	GLFWwindow * window;


public:
	Window(const char * title, size_t width, size_t height);
	~Window();

	void use();
	bool isOpen();
	void swapBuffers();

	static void pollEvents();
};

