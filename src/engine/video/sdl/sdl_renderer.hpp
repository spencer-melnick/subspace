#pragma once

// STL includes
#include <vector>

// Library includes
#ifdef _WIN32
	#include <SDL_render.h>
#else
	#include <SDL2/SDL_render.h>
#endif

// Project includes
#include "engine/video/renderer.hpp"
#include "sdl_window.hpp"
#include "sprite.hpp"

namespace subspace {
	class SdlRenderer: public IRenderer {
		public:
			SdlRenderer(const SdlWindow& window);
			~SdlRenderer();

			// Interface functions
			virtual void draw() override;

			// Member functions
			SdlSprite& createSprite();

		private:
			const SdlWindow& window_;
			SDL_Renderer* handle_;
			std::vector<SdlSprite> sprites_;
	};
}
