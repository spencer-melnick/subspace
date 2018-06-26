#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Class to wrap Vulkan a Vulkan instance, surface, and associated functions.
 */

#include <memory>

namespace subspace {
    /**
     * Class to wrap Vulkan a Vulkan instance, and associated functions.
     */

    class VulkanInstance;
    class VulkanPhysicalDevice;
    class VulkanLogicalDevice;

    class RenderContext {
        public:
            /**
             * Creates a render context wrapping an underlying Vulkan instance with all 
             * extensions required by a given SDL window.
             * 
             * @param window The window with which to query required extensions.
             */
            RenderContext();
            ~RenderContext();

            const VulkanInstance& getInstance() const;
            const VulkanPhysicalDevice& getPhysicalDevice() const;
            const VulkanLogicalDevice& getLogicalDevice() const;

        private:
            std::unique_ptr<VulkanInstance> instance_;
            const VulkanPhysicalDevice* physicalDevice_;
            std::unique_ptr<VulkanLogicalDevice> logicalDevice_;
    };
}
