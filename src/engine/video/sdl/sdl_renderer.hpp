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

namespace subspace {
	class SdlRenderer: public IRenderer {
		public:
			SdlRenderer(const SdlWindow& window);
			~SdlRenderer();

			// Interface functions
			virtual void draw() override;

		private:
			const SdlWindow& window_;
			SDL_Renderer* handle_;
	};
}
