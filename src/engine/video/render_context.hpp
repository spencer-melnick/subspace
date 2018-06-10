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
     * Class to wrap Vulkan a Vulkan instance, and associated functions.
     */
    class RenderContext {
        public:
            /**
             * Creates a render context wrapping an underlying Vulkan instance with all 
             * extensions required by a given SDL window.
             * 
             * @param window The window with which to query required extensions.
             */
            RenderContext(const Window& window);
            ~RenderContext();

        private:
            vk::Instance vulkanInstance_;

            vk::Instance createVulkanInstance(const Window& window);
            static std::vector<const char*> getRequiredExtensions(const Window& window);
    };
}
