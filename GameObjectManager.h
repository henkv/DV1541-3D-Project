#pragma once
#include <vector>
#include "GameObject.h"
#include "Camera.h"
#include "Model.h"

class GameObjectManager : GameObject
{
private:
	typedef std::map<uid_t, GameObject*> map_t;
	map_t objects;

	//help
	bool sort(Camera & camera, GameObject* objectLeft, GameObject* objectRight);
	std::vector<GameObject*> objects;
public:
	GameObjectManager();
	~GameObjectManager();

	void add(GameObject* object);
	void remove(uid_t objectId);
	int size();
	void frontToBackSort(Camera & camera);


	// Inherited via GameObject
	virtual void draw(Shader & shader) override;
	virtual void update(float delta) override;

	virtual void drawTesselated(Shader & shader) override;
};

