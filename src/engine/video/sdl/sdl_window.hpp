#pragma once

// STL includes
#include <string>

// Library includes
#ifdef _WIN32
	#include <SDL.h>
	#include <SDL_video.h>
#elif
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_video.h>
#endif

// Project includes
#include "engine/video/window.hpp"
#include "engine/util/config.hpp"

namespace subspace {
	class SdlWindow: public IWindow {
		public:
			SdlWindow(const std::string& name, const Config& config);
			~SdlWindow();

			// Interface functions
			virtual void swap() override;

		private:
			std::string name_;
			SDL_Window* handle_;
	};
}
