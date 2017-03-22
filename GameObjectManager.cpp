#include "GameObjectManager.h"



GameObjectManager::GameObjectManager()
{
}


GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::add(GameObject* object)
{
	objects.push_back(object);
}



void GameObjectManager::update(float delta)
{
	for (auto &object : objects)
	{
		object->update(delta);
	}
}

void GameObjectManager::drawTesselated(Shader & shader)
{
	for (auto &object : objects)
	{
		object->drawTesselated(shader);
	}
}

void GameObjectManager::draw(Shader & shader)
{
	for (auto &object : objects)
	{
		object->draw(shader);
	}
}
