#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Class to wrap Vulkan a Vulkan instance, surface, and associated functions.
 */

#include <memory>

namespace subspace {
    class Window;
    class Shader;

    /**
     * Class to wrap Vulkan a Vulkan instance, and associated functions.
     */
    class RenderContext {
        public:
            struct InstanceHandle;
            struct DeviceHandle;

            /**
             * Creates a render context wrapping an underlying Vulkan instance with all 
             * extensions required by a given SDL window.
             * 
             * @param window The window with which to query required extensions.
             */
            RenderContext();
            ~RenderContext();

            const InstanceHandle& getInstance() const;
            const DeviceHandle& getDevice() const;

        private:
            class Impl;
            std::unique_ptr<Impl> impl_;
    };
}
