#pragma once

#include <vulkan/vulkan.hpp>

namespace subspace {
    class VulkanLogicalDevice;

    class VulkanPipelineLayout {
        public:
            VulkanPipelineLayout(const VulkanLogicalDevice& device);
            ~VulkanPipelineLayout();

            operator const vk::PipelineLayout&() const;

        private:
            vk::PipelineLayout handle_;
            const VulkanLogicalDevice& device_;
    };
}
