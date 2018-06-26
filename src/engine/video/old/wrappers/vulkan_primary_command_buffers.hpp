#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace subspace {
    class RenderContext;
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanPipeline;
    class VulkanFramebuffers;

    class VulkanPrimaryCommandBuffers {
        public:
            VulkanPrimaryCommandBuffers(const RenderContext& context,
                const VulkanSwapchain& swapchain, const VulkanPipeline& pipeline,
                const VulkanFramebuffers& framebuffers);
            ~VulkanPrimaryCommandBuffers();

        private:
            const VulkanLogicalDevice& device_;

            vk::CommandPool pool_;
            std::vector<vk::CommandBuffer> handles_;

            void recordCommands(const VulkanSwapchain& swapchain, const VulkanPipeline& pipeline,
                const VulkanFramebuffers& framebuffers);
    };
}
