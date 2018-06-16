#pragma once

#include <vector>
#include <map>
#include <functional>

#include <vulkan/vulkan.hpp>

#include "vulkan_physical_device.hpp"

namespace subspace {
    class SdlWindow;

    class VulkanInstance {
        public:
            using PhysicalDeviceList = std::multimap<unsigned, VulkanPhysicalDevice, std::greater<unsigned>>;

            VulkanInstance();
            ~VulkanInstance();

            const vk::Instance& getHandle() const;
            const PhysicalDeviceList& getPhysicalDevices() const;

        private:
            void queryPhysicalDevices(const SdlWindow& dummyWindow);

            vk::Instance handle_;
            PhysicalDeviceList physicalDevices_;
    };
}
