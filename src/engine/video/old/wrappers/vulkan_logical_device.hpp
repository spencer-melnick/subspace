#pragma once

#include <vulkan/vulkan.hpp>

#include "vulkan_instance.hpp"
#include "vulkan_physical_device.hpp"

namespace subspace {
    class VulkanLogicalDevice {
        public:
            VulkanLogicalDevice(const VulkanPhysicalDevice& physicalDevice);
            ~VulkanLogicalDevice();

            operator const vk::Device&() const;
            const vk::Device* operator->() const;

        private:
            vk::Device handle_;
    };
}
