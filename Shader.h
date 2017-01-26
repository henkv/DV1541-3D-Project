#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>

using namespace glm;

class Shader
{
private:
	static GLuint activeProgram;
	GLuint program;

	GLuint loadShader(GLenum shaderType, const GLchar * filePath);
public:
	Shader(const char * vertexPath, 
		   const char * fragmentPath);
	Shader(const char * vertexPath,
		   const char * geometryPath,
		   const char * fragmentPath);
	~Shader();

	void use();
	GLint getUniform(const GLchar * name);
	void setUniform(const GLchar * name, const float & value);
	void setUniform(const GLchar * name, const vec2 & vector);
	void setUniform(const GLchar * name, const vec3 & vector);
	void setUniform(const GLchar * name, const mat4 & matrix);
};
