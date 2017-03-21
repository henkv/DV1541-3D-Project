#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <map>
#include "Input.h"

class Window
{
private:
	static void mouseButton(GLFWwindow* window, int button, int action, int mods);
	static void mouseMove(GLFWwindow* window, double xpos, double ypos);
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

