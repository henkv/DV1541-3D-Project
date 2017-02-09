#pragma once
#include <map>
#include "Light.h"
#include "GameObject.h"

class LightManager : public GameObject
{
public:
	typedef std::map<const Light::uid_t, Light*> map_t;
	map_t lights;

public:
	LightManager();
	~LightManager();

	void add(Light * light);

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update(float delta) override;
};

