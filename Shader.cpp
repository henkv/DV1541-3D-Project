#include "Shader.h"
#include <fstream>
#include <glm\gtc\type_ptr.hpp>

GLuint Shader::activeProgram = GL_NONE;

GLuint Shader::loadShader(GLenum shaderType, const GLchar * filePath)
{
	GLuint shader = glCreateShader(shaderType);

	std::ifstream file(filePath);
	std::string text = { std::istreambuf_iterator<char>(file),  std::istreambuf_iterator<char>() };
	file.close();

	const GLchar * textPtr = text.c_str();
	glShaderSource(shader, 1, &textPtr, NULL);
	glCompileShader(shader);


	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLchar shaderLog[512] = { NULL };
		glGetShaderInfoLog(shader, 512, NULL, shaderLog);
		throw shaderLog;
	}

	return shader;
}

Shader::Shader(const char * vertexPath, 
			   const char * fragmentPath)
{
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLchar programLog[512] = { NULL };
		glGetProgramInfoLog(program, 512, NULL, programLog);
		throw programLog;
	}
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::use()
{
	glUseProgram(program);
	activeProgram = program;
}

GLint Shader::getUniform(const GLchar * name)
{
	if (activeProgram != program)
		throw "cant get uniform, shader not active";
	return glGetUniformLocation(program, name);
}

void Shader::setUniform(const GLchar * name, const mat4 & matrix)
{
	glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, value_ptr(matrix));
}
