#include "window.hpp"

namespace subspace {
    Window::Window(const std::string& name, const Config& config) {
        Uint32 flags = 0;

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
            throw VideoException(VideoException::Type::WINDOW_CREATE_FAILURE);
        }
    }

    Window::~Window() {
        SDL_DestroyWindow(sdlWindow_);
    }
}
