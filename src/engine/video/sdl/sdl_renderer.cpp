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

	// Create new texture factory
	textureFactory_ = std::make_unique<TextureFactory>(*this);
}

SdlRenderer::~SdlRenderer() {
	SDL_DestroyRenderer(handle_);
	logger.logVerbose("Destroyed SDL renderer");
}

SdlRenderer::operator SDL_Renderer*() const {
	return handle_;
}

void SdlRenderer::draw() {
	SDL_RenderClear(handle_);

	// For each sprite, render
	for (auto& i : sprites_) {
		auto& texture = *i.texture;

		Vec2 size {texture.dimension.x * i.scale.x, texture.dimension.y * i.scale.y};
		Vec2 topLeft = i.position - (size / 2.0);
		SDL_Rect dest {topLeft.x, topLeft.y, size.x, size.y};

		if (SDL_RenderCopyEx(handle_, texture.handle, nullptr, &dest, 0.0, nullptr, SDL_FLIP_NONE) != 0) {
			logger.logWarning("Failed to render sprite - SDL error: {}", SDL_GetError());
		}
	}

	SDL_RenderPresent(handle_);
}

SdlSprite& SdlRenderer::createSprite() {
	sprites_.emplace_back();
	return sprites_.back();
}

void SdlRenderer::destroySprite(SdlSprite& sprite) {
	for (auto i = sprites_.begin(); i != sprites_.end(); i++) {
		if (&(*i) == &sprite) {
			sprites_.erase(i);
			break;
		}
	}
}

TextureFactory& SdlRenderer::getTextureFactory() {
	return *textureFactory_;
}
