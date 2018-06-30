#pragma once

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

namespace subspace {
    class SdlWindow;
    class VulkanInstance;

    class VulkanSurface {
        public:
            VulkanSurface(const VulkanInstance& instance, const SdlWindow& window);
            ~VulkanSurface();

            operator VkSurfaceKHR() const;
            operator const vk::SurfaceKHR&() const;

        private:
            const VulkanInstance& instance_;
            vk::SurfaceKHR handle_;
    };
}
