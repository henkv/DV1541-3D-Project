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

int GameObjectManager::size()
{
	int result = 0;
	for (auto &object : objects)
	{
		result++;
	}
	return result;
}

GameObject * GameObjectManager::getObjectPointer(int id)
{
	GameObject * targetObject = nullptr;
	targetObject = objects.at(id);
	return targetObject;
}

void GameObjectManager::setObjectPointer(int id, GameObject * object)
{
	objects.at(id) = object;
}

void GameObjectManager::update(float delta)
{
	for (auto &object : objects)
	{
		object.second->update(delta);
	}
}

void GameObjectManager::draw(Shader & shader)
{
	for (auto &object : objects)
	{
		object.second->draw(shader);
	}
}
