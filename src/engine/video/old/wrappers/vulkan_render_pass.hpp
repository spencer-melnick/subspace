#pragma once

#include <vulkan/vulkan.hpp>

namespace subspace {
    class VulkanLogicalDevice;
    class VulkanSwapchain;

    class VulkanRenderPass {
        public:
            VulkanRenderPass(const VulkanLogicalDevice& device, const VulkanSwapchain& swapchain);
            ~VulkanRenderPass();

            operator const vk::RenderPass&() const;

        private:
            vk::RenderPass handle_;
            const VulkanLogicalDevice& device_;
    };
}
