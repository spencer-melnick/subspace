#include "window.hpp"

#include <fmt/format.h>

#include "../util/logger.hpp"

namespace subspace {
    Window::Window(const std::string& name, const Config& config) {
        Uint32 flags = SDL_WINDOW_VULKAN;

        if (config.isFullscreen()) {
            flags = SDL_WINDOW_FULLSCREEN;
        }

        sdlWindow_ = SDL_CreateWindow(
            name.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            config.getXResolution(), config.getYResolution(),
            flags
        );

        if (sdlWindow_ == nullptr) {
            logger.logError(fmt::format("SDL2 error: {}", SDL_GetError()));
            throw VideoException(VideoException::Type::WindowCreateFailure);
        }

        logger.logDebug("Created SDL2 window");
    }

    Window::~Window() {
        logger.logDebug("Destroyed SDL2 window");
        SDL_DestroyWindow(sdlWindow_);
    }
}
