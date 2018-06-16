#pragma once

#include <map>

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL_vulkan.h>

#include "video_exception.hpp"
#include "render_context.hpp"

namespace subspace {
    class Window;
	class SwapChain;

    class RenderContext::Impl_ {
        public:
            Impl_();
            ~Impl_();

        private:
            friend class Window;
			friend class SwapChain;

            struct DeviceWrapper_ {
                vk::PhysicalDevice vulkanDevice;
                unsigned usedQueueFamily;
                std::string name;
            };

            using DeviceList_ = std::multimap<unsigned, DeviceWrapper_>;

            vk::Instance instance_;
            DeviceWrapper_ physicalDevice_;
            vk::Device device_;
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
            static int rateDeviceSuitability(DeviceWrapper_& physicalDevice);
            static vk::Device createLogicalDevice(DeviceWrapper_& physicalDevice);
            static bool checkDeviceExtensions(vk::PhysicalDevice& device);
            static bool findQueueFamily(DeviceWrapper_& device, vk::SurfaceKHR& surface);
    };
}
