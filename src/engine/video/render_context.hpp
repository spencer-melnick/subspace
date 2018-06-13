#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Class to wrap Vulkan a Vulkan instance, surface, and associated functions.
 */

#include <vector>
#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

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
            RenderContext();
            ~RenderContext();

        private:
            struct DeviceWrapper_ {
                vk::PhysicalDevice vulkanDevice;
                unsigned usedQueueFamily;
                std::string name;
            };

            using DeviceList_ = std::multimap<unsigned, DeviceWrapper_>;

            vk::Instance vulkanInstance_;
            vk::Device device_;

            DeviceList_ getSupportedDevices(vk::SurfaceKHR& surface);
            static vk::Instance createVulkanInstance(SDL_Window* window);
            static std::vector<const char*> getRequiredExtensions(SDL_Window* window);
            static int rateDeviceSuitability(DeviceWrapper_& physicalDevice);
            static vk::Device createLogicalDevice(DeviceWrapper_& physicalDevice);
            static bool findQueueFamily(DeviceWrapper_& physicalDevice, vk::SurfaceKHR& surface);
    };
}
