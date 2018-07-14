#include "sdl_renderer.hpp"

// STL includes
#include <exception>

// Project includes
#include "engine/util/logger.hpp"
#include "sdl_window.hpp"

using namespace std;
using namespace subspace;

SdlRenderer::SdlRenderer(const SdlWindow& window) :
	window_(window)
{
	// Create whatever SDL renderer is available first
	handle_ = SDL_CreateRenderer(window, -1, 0);

	if (handle_ == nullptr) {
		logger.logError("SDL Error: {}", SDL_GetError());
		throw runtime_error("Failed to create SDL renderer");
	}

	SDL_SetRenderDrawColor(handle_, 40, 40, 40, 255);
}

SdlRenderer::~SdlRenderer() {
	SDL_DestroyRenderer(handle_);
	logger.logVerbose("Destroyed SDL renderer");
}

void SdlRenderer::draw() {
	SDL_RenderClear(handle_);

	// For each sprite, render

	SDL_RenderPresent(handle_);
}
