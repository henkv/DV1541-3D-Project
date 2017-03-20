#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <map>
#include "Input.h"

class Window
{
private:
	static void error(int error, const char * message);
	static void key(GLFWwindow* window, int key, int scancode, int action, int mods);

	static int nrOfWindows;
	GLFWwindow * window;


public:
	Window(const char * title, size_t width, size_t height);
	~Window();

	void use();
	void swapBuffer();

	bool isOpen();

	static void pollEvents();
	static double getTime();
};

