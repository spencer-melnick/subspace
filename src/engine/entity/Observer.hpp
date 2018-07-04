#pragma once
#include "Event.hpp"

class Observer {
public:
	virtual ~Observer();
	virtual void onNotify(Event event);
};