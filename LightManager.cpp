#include "LightManager.h"



LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}

void LightManager::add(Light * light)
{
	lights.insert(map_t::value_type(light->UID, light));
}

void LightManager::draw(Shader & shader)
{
}

void LightManager::update(float delta)
{
	for (auto & light : lights)
	{
		light.second->update(delta);
	}
}
