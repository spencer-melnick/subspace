#pragma once

// Project includes
#include "event.hpp"

namespace subspace {
	class Observer {
		public:
			virtual ~Observer();
			virtual void onNotify(Event event);
	};
}