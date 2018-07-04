#pragma once
#include <forward_list>

#include "Observer.hpp"
#include "Event.hpp"

using namespace std;

class Subject {
public:
	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);
private:
	forward_list<Observer*> _observers;
protected:
	void notify(Event event);
};