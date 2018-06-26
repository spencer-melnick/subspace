#pragma once

// STL includes
#include <string>
#include <vector>

// Library includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

// Project includes 
#include "engine/util/config.hpp"

namespace subspace {
    class SdlWindow {
        public:
            SdlWindow(const std::string& name, unsigned displayNum, unsigned width,
                unsigned height, uint32_t flags);
            ~SdlWindow();

            // Member functions
            std::vector<const char*> listRequiredExtensions() const;
            vk::SurfaceKHR createSurface(const vk::Instance& instance) const;

            // Conversion to wrapped type
            operator SDL_Window*() const;

            // Getters/Setters
            const std::string& getName() const;
            unsigned getWidth() const;
            unsigned getHeight() const;

        private:
            std::string name_;
            unsigned displayNum_, width_, height_;

            SDL_Window* handle_;
    };
}