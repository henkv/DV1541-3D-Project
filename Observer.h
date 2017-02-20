#pragma once

template<typename T>
class Observer
{
public:
	typedef Observer<std::pair<bool, size_t>> observer_t;
	typedef T event_t;

	virtual void onNotify(event_t & event) = 0;
};

