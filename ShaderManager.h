#pragma once
#include "Shader.h"
#include <map>

using namespace std;

class ShaderManager
{
private:
	map<string, Shader> shaders;
	
public:
	ShaderManager();
	~ShaderManager();

	void addShader(const char * name, const char * vertexShader, const char * fragmentShader);
	void addShader(const char * name, const char * vertexShader, const char * fragmentShader, const char * geometryShader);

	void useShader(const char * name);
	Shader * getShader(const char * name);
};

