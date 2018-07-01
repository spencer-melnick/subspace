#include "Subject.hpp"

#include <forward_list>

using namespace std;

void Subject::addObserver(Observer* observer) {
	_observers.push_front(observer);
}

void Subject::removeObserver(Observer* observer) {
	_observers.remove(observer);
}

void Subject::notify(Event event) {
	for (auto iterator = _observers.begin(); iterator != _observers.end(); iterator++) {
		(*iterator)->onNotify(event);
	}
}