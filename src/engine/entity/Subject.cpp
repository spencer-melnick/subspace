#include "subject.hpp"

using namespace std;
using namespace subspace;

void Subject::addObserver(Observer* observer) {
	observers_.push_back(observer);
}

void Subject::removeObserver(Observer* observer) {
	observers_.remove(observer);
}

void Subject::notify(Event event) {
	for (auto& i : observers_) {
		i->onNotify(event);
	}
}