#pragma once

// STL includes
#include <vector>

// Library includes
#include <vulkan/vulkan.hpp>

namespace subspace {
    struct Swapchain {
        vk::UniqueSwapchainKHR handle;
        std::vector<vk::Image> images;
        std::vector<vk::ImageView> imageViews;
        vk::Extent2D extent;
        std::vector<vk::Framebuffer> framebuffers;

        static Swapchain fromDevice(vk::PhysicalDevice& device, vk::SurfaceKHR& surface);
    };
}
