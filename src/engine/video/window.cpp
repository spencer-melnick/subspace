#include "window.hpp"

#include <SDL2/SDL.h>

#include "../util/logger.hpp"

using namespace subspace;

class Window::Impl_ {
    public:
        Impl_(const std::string& name, const Config& config) {
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
                logger.logError("SDL2 error: {}", SDL_GetError());
                throw VideoException(VideoException::Type::WindowCreateFailure);
            }

            logger.logDebug("Created SDL2 window");
        }

        ~Impl_() {
            logger.logDebug("Destroyed SDL2 window");
            SDL_DestroyWindow(sdlWindow_);
        }
    private:
        SDL_Window* sdlWindow_;
};

Window::Window(const std::string& name, const Config& config) :
    impl_(new Impl_(name, config))
{}

Window::~Window() = default;
