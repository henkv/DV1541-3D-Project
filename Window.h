#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <map>


class Window
{
private:
	static class CallbackHandler
	{
	public:
		static void error(int error, const char * message);
		static void key(GLFWwindow* window, int key, int scancode, int action, int mods);

		std::map<GLFWwindow*, Window*> windows;
		void subscribe(GLFWwindow* window, Window* windowObj);
		void unsubscribe(GLFWwindow* window);

		CallbackHandler();
	} callbacks;

	static int nrOfWindows;
	GLFWwindow * window;


public:
	Window(const char * title, size_t width, size_t height);
	~Window();

	void use();
	void swapBuffers();

	bool isOpen();
	bool getKey(int key);


	static void pollEvents();
	static double getTime();
};

