#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace subspace {
    class VulkanLogicalDevice;    
    class VulkanSwapchain;
    class VulkanRenderPass;

    class VulkanFramebuffers {
        public:
            VulkanFramebuffers(const VulkanLogicalDevice& device,
                const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass);
            ~VulkanFramebuffers();

            operator const std::vector<vk::Framebuffer>&() const;

        private:
            const VulkanLogicalDevice& device_;
            const VulkanSwapchain& swapchain_;
            const VulkanRenderPass& renderPass_;

            std::vector<vk::Framebuffer> handles_;
    };
}
