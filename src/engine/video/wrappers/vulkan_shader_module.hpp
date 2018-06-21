#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace subspace {
    class VulkanLogicalDevice;

    class VulkanShaderModule {
        public:
            VulkanShaderModule(const VulkanLogicalDevice& device, const std::string& filename);
            ~VulkanShaderModule();

            operator const vk::ShaderModule&() const;

        private:
            vk::ShaderModule handle_;
            const VulkanLogicalDevice& device_;
    };
}