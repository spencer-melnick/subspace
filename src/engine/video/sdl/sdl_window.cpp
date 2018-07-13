#include "sdl_window.hpp"

// STL includes
#include <exception>

// Project includes
#include "engine/util/logger.hpp"

using namespace std;
using namespace subspace;

SdlWindow::SdlWindow(const string& name, const Config& config) :
	name_(name)
{
	uint32_t windowFlags = 0;
	int windowPos = SDL_WINDOWPOS_UNDEFINED_DISPLAY(0);

	unsigned width = config.getXResolution();
	unsigned height = config.getYResolution();

	if (config.isFullscreen()) {
		windowFlags |= SDL_WINDOW_FULLSCREEN;
	}

	handle_ = SDL_CreateWindow(name_.c_str(), windowPos, windowPos, width, height, windowFlags);

	if (handle_ == nullptr) {
		logger.logError("SDL Error: {}", SDL_GetError());
		throw std::runtime_error("Failed to create SDL window");
	}
}

SdlWindow::~SdlWindow() {
	SDL_DestroyWindow(handle_);
	logger.logVerbose("Destroyed window {}", name_);
}

void SdlWindow::swap() {

}