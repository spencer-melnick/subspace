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

            vk::Instance instance_;
            vk::Device device_;
            vk::Queue queue_;

            static const std::vector<const char*> requiredDeviceExtensions_;

            DeviceList_ getSupportedDevices(vk::SurfaceKHR& surface);
            static SDL_Window* createDummyWindow();
            static vk::SurfaceKHR createDummySurface(SDL_Window* window, vk::Instance& instance);
            static vk::Instance createVulkanInstance(SDL_Window* window);
            static std::vector<const char*> getRequiredWindowExtensions(SDL_Window* window);
            static int rateDeviceSuitability(DeviceWrapper_& physicalDevice);
            static vk::Device createLogicalDevice(DeviceWrapper_& physicalDevice);
            static bool checkDeviceExtensions(vk::PhysicalDevice& device);
            static bool findQueueFamily(DeviceWrapper_& device, vk::SurfaceKHR& surface);
    };
}
