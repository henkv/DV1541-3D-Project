#pragma once
#include <map>
#include "GameObject.h"

class GameObjectManager : GameObject
{
private:
	typedef std::map<uid_t, GameObject*> map_t;
	map_t objects;
public:
	GameObjectManager();
	~GameObjectManager();

	void add(GameObject* object);
	void remove(uid_t objectId);
	int size();
	GameObject * getObjectPointer(int id);
	void setObjectPointer(int id, GameObject * object);

	// Inherited via GameObject
	virtual void draw(Shader & shader) override;
	virtual void update(float delta) override;
};

