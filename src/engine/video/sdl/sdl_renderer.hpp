#pragma once

// STL includes
#include <vector>

// Library includes
#include <SDL_render.h>

// Project includes
#include "engine/video/renderer.hpp"
#include "sdl_window.hpp"
#include "sprite.hpp"

namespace subspace {
	class SdlRenderer: public IRenderer {
		public:
			SdlRenderer(const SdlWindow& window);
			~SdlRenderer();

			// Cast to wrapped type
			operator SDL_Renderer*();

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
