#pragma once

#include <map>

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL_vulkan.h>

#include "video_exception.hpp"
#include "render_context.hpp"

namespace subspace {
    struct RenderContext::InstanceHandle {
        vk::Instance instance;
    };

    struct RenderContext::DeviceHandle {
        vk::PhysicalDevice physicalDevice;
        vk::Device logicalDevice;
        unsigned usedQueueFamily;
        std::string deviceName;
    };

    class RenderContext::Impl {
        public:
            Impl();
            ~Impl();

            const InstanceHandle& getInstance() const;
            const DeviceHandle& getDevice() const;

        private:

            using DeviceList_ = std::multimap<unsigned, DeviceHandle>;

            InstanceHandle instance_;
            DeviceHandle device_;
            vk::Queue queue_;

            static const std::vector<const char*> requiredDeviceExtensions_;

            // Hoo boy!
            // This got a little out out hand - creating Vulkan a Vulkan instance, device,and queue
            // takes a lot of steps. It's all broken down into separate static functions to make it 
            // more readable.
            DeviceList_ getSupportedDevices(vk::SurfaceKHR& surface);

            static SDL_Window* createDummyWindow();
            static vk::SurfaceKHR createDummySurface(SDL_Window* window, vk::Instance& instance);
            static vk::Instance createVulkanInstance(SDL_Window* window);
            static std::vector<const char*> getRequiredWindowExtensions(SDL_Window* window);
            static int rateDeviceSuitability(vk::PhysicalDevice& physicalDevice);
            static vk::Device createLogicalDevice(vk::PhysicalDevice& physicalDevice,
                uint32_t queueFamily);
            static bool checkDeviceExtensions(vk::PhysicalDevice& device);
            static int32_t findQueueFamily(vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface);
    };
}
