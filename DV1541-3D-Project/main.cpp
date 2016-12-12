#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

using namespace glm;

inline void die(unsigned int line, const char * errorMessage);

int main()
{


	if (!glfwInit()) die(__LINE__, "failed to init glfw");
	GLFWwindow* window = glfwCreateWindow(800, 600, "DV1541 3D Project", NULL, NULL);
	if (!window) die(__LINE__, "failed to create window");


	glfwDestroyWindow(window);
	system("pause");
	return 0;
}

inline void die(unsigned int line, const char * errorMessage)
{
	printf("%d: %s\n", line, errorMessage);
	glfwTerminate();
	system("pause");
	exit(-1);
}