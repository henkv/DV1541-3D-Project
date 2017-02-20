#pragma once
#include <unordered_set>
#include "Observer.h"

template<typename T>
class Subject
{
private:
	std::unordered_set<Observer*> observers;
	
public:
	void subscribe(Observer & observer)
	{
		observers.insert(&observer);
	}

	void unsubscribe(Observer & observer)
	{
		observers.erase(observers.find(&observer));
	}

	void notify(T & event)
	{
		for (auto & observer : observers)
		{
			observer->onNotify(event);
		}
	}
};

