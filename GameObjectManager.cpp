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

void GameObjectManager::frontToBackSort(Camera & camera)
{
	bool swapped;
	for (int i = 0; i < objects.size() - 1; i++)
	{
		swapped = false;
		for (int j = 0; j < objects.size() - 1; j++)
		{
			if (this->sort(camera, objects[j], objects[j + 1]))
			{
				GameObject * temp = objects[j];
				objects[j] = objects[j + 1];
				objects[j + 1] = temp;
				swapped = true;
			}
			//https://www.gamedev.net/topic/567728-front-to-back-rendering/
		}
		if (swapped == false)
		{
			break;
		}
	}
}


bool GameObjectManager::sort(Camera & camera, GameObject * objectLeft, GameObject * objectRight )
{
	Model* left = dynamic_cast<Model*>(objectLeft);
	Model* right = dynamic_cast<Model*>(objectRight);

	vec3 cameraPos = camera.getPosition();

	vec3 leftPos = left == nullptr ? cameraPos : left->getPosition();
	vec3 rightPos = right == nullptr ? cameraPos : right->getPosition();

	float leftDistance = length(leftPos - cameraPos);
	float rightDistance = length(rightPos - cameraPos);

	return leftDistance > rightDistance;
}
