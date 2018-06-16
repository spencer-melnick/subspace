#pragma once

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

namespace subspace {
    class SdlWindow {
        public:
            SdlWindow(const std::string& name, unsigned displayNum, unsigned width, unsigned height, uint32_t flags);
            ~SdlWindow();

            SDL_Window* getHandle() const;
            std::vector<const char*> listRequiredExtensions() const;

        private:
            SDL_Window* handle_;
    };
}
