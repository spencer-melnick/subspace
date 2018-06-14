#pragma once

#include <map>

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL_vulkan.h>

#include "video_exception.hpp"
#include "render_context.hpp"

namespace subspace {
    class Window;

    class RenderContext::Impl_ {
        public:
            Impl_();
            ~Impl_();

        private:
            friend class Window;

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
            static bool findQueueFamily(DeviceWrapper_& device, vk::SurfaceKHR& surface);
    };
}
