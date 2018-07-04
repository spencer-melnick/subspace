#pragma once

// STL includes
#include <forward_list>

// Project includes
#include "observer.hpp"
#include "event.hpp"

namespace subspace {
	class Subject {
		public:
			void addObserver(Observer* observer);
			void removeObserver(Observer* observer);
		private:
			std::forward_list<Observer*> observers_;
		protected:
			void notify(Event event);
	};
}