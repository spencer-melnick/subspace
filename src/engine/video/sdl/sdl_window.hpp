#pragma once

// STL includes
#include <string>

// Library includes
#include <SDL.h>
#include <SDL_video.h>

// Project includes
#include "engine/video/window.hpp"
#include "engine/util/config.hpp"

namespace subspace {
	class SdlWindow: public IWindow {
		public:
			SdlWindow(const std::string& name, const Config& config);
			~SdlWindow();

			// Cast to wrapped type
			operator SDL_Window*() const;

			// Interface functions
			virtual void swap() override;

		private:
			std::string name_;
			SDL_Window* handle_;
	};
}
