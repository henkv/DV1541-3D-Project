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



int GameObjectManager::size()
{
	int result = 0;
	for (auto &object : objects)
	{
		result++;
	}
	return result;
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
	bool result = false;
	float left;
	float xLeft = camera.getPosition().x - dynamic_cast<Model*>(objectLeft)->getPosition().x;
	xLeft = xLeft * xLeft;
	float yLeft = camera.getPosition().y - dynamic_cast<Model*>(objectLeft)->getPosition().y;
	yLeft = yLeft * yLeft;
	float zLeft = camera.getPosition().z - dynamic_cast<Model*>(objectLeft)->getPosition().z;
	zLeft = zLeft * zLeft;
	left = xLeft + yLeft + zLeft;
	left = sqrt(left);

	float right;
	float xRight = camera.getPosition().x - dynamic_cast<Model*>(objectRight)->getPosition().x;
	xRight = xRight * xRight;
	float yRight = camera.getPosition().y - dynamic_cast<Model*>(objectRight)->getPosition().y;
	yRight = yRight * yRight;
	float zRight = camera.getPosition().z - dynamic_cast<Model*>(objectRight)->getPosition().z;
	zRight = zRight * zRight;
	right = xRight + yRight + zRight;
	right = sqrt(right);

	if (left > right)
	{
		result = true;
	}

	return result;
}
