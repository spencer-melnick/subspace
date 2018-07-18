#pragma once

// Library includes
#include <SDL_render.h>

// Project includes
#include "engine/math/math.hpp"

namespace subspace {
	struct SdlSprite {
		Vec2 position, scale;
		Vec2i source;
		SDL_Texture* texture;
	};
}
