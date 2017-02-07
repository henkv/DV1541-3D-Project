#include "ShaderManager.h"



ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}

void ShaderManager::addShader(const char * name, const char * vertexShader, const char * fragmentShader)
{
	shaders.insert(make_pair(name, Shader(vertexShader, fragmentShader)));
}

void ShaderManager::addShader(const char * name, const char * vertexShader, const char * geometryShader, const char * fragmentShader)
{
	shaders.insert(make_pair(name, Shader(vertexShader, geometryShader, fragmentShader)));
}

void ShaderManager::useShader(const char * name)
{
	shaders.at(name).use();
}

Shader * ShaderManager::getShader(const char * name)
{
	return &shaders.at(name);
}

