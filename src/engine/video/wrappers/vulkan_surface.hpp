#pragma once

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

namespace subspace {
    class VulkanSurface {
        public:
            VulkanSurface(const vk::Instance& instance, SDL_Window* window);
            ~VulkanSurface();

            const vk::SurfaceKHR& getHandle() const;

        private:
            const vk::Instance& instance_;
            vk::SurfaceKHR handle_;
    };
}
