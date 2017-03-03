#include "LightManager.h"



LightManager::LightManager()
{
	bufferData.nrOfLights = 0;

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(int) + sizeof(Light) * 100, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


LightManager::~LightManager()
{
}

void LightManager::add(Light & light)
{
	
	bufferData.lights[bufferData.nrOfLights++] = light;

	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(bufferData), &bufferData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void LightManager::draw(Shader & shader)
{
}


void LightManager::update(float delta)
{

}
