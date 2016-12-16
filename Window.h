#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <map>


class Window
{
private:
	static class CallbackHandler
	{
	private:
		std::map<GLFWwindow*, Window*> windows;

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

	void keyEvent(int key, int action, int mods);

	static void pollEvents();
};

