#pragma once

// Library includes
#ifdef _WIN32
	#include <SDL_render.h>
#else
	#include <SDL2/SDL_render.h>
#endif

// Project includes
#include "engine/math/math.hpp"

namespace subspace {
	struct SdlSprite {
		Vec2 position, scale;
		Vec2i source;
		SDL_Texture* texture;
	};
}
