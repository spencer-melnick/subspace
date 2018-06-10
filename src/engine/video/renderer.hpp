#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Class to wrap Vulkan a Vulkan instance, surface, and associated functions.
 */

#include <vector>

#include <vulkan/vulkan.hpp>

#include "window.hpp"

namespace subspace {
    /**
     * Class to wrap Vulkan a Vulkan instance, surface, and associated functions.
     */
    class Renderer {
        public:
            /**
             * Creates a renderer attached to the specified window and creates the underlying
             * Vulkan instance and surface required for that window.
             * 
             * @param window The window to which the renderer is to be attached.
             */
            Renderer(const Window& window);
            ~Renderer();

        private:
            vk::Instance vulkanInstance_;

            vk::Instance createVulkanInstance(const Window& window);
            static std::vector<const char*> getRequiredExtensions(const Window& window);
    };
}
