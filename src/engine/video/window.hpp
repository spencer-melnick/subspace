#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Class that provides simple window functionality by wrapping the SDL2 window
 * interface.
 */

#include <string>

#include <SDL2/SDL.h>

#include "video_exception.hpp"
#include "../util/config.hpp"

namespace subspace {
    class Window {
        /**
         * Class that provides simple window functionality by wrapping the SDL2 window
         * interface.
         */

        public:
            Window(const std::string& name, const Config& config);
            ~Window();

        private:
            SDL_Window* sdlWindow_;
    };
}
