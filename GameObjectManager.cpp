#include "GameObjectManager.h"



GameObjectManager::GameObjectManager()
{
}


GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::add(GameObject* object)
{
	objects.insert(map_t::value_type(object->UID, object));
}

void GameObjectManager::remove(uid_t objectId)
{
	objects.erase(objects.find(objectId));
}

void GameObjectManager::update(float delta)
{
	for (auto &object : objects)
	{
		object.second->update(delta);
	}
}

void GameObjectManager::draw()
{
	for (auto &object : objects)
	{
		object.second->draw();
	}
}
