#pragma once
#include <glad\glad.h>


class Shader
{
private:
	GLuint program;

	GLuint loadShader(GLenum shaderType, const GLchar * filePath);
public:
	Shader(const char * vertexPath, 
		   const char * fragmentPath);
	~Shader();

	void use();
};

