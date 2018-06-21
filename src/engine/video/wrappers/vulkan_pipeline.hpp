#pragma once

#include <vulkan/vulkan.hpp>

#include "vulkan_render_pass.hpp"
#include "vulkan_pipeline_layout.hpp"

namespace subspace {
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanShaderModule;

    class VulkanPipeline {
        public:
            VulkanPipeline(const VulkanLogicalDevice& device, const VulkanSwapchain& swapchain,
                const VulkanShaderModule& vertShader, const VulkanShaderModule& fragShader);
            ~VulkanPipeline();

            operator const vk::Pipeline&() const;

        private:
            vk::Pipeline handle_;

            const VulkanLogicalDevice& device_;
            VulkanRenderPass renderPass_;
            VulkanPipelineLayout layout_;
    };
}
