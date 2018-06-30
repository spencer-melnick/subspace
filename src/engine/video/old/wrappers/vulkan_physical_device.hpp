#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace subspace {
    class VulkanInstance;
    class VulkanSurface;

    class VulkanPhysicalDevice {
        public:
            operator const vk::PhysicalDevice&() const;
            const vk::PhysicalDevice* operator->() const;

            bool isUsable() const;
            unsigned getUsedQueueFamily() const;
            unsigned getRating() const;
            const std::string& getName() const;

            static const std::vector<const char*> requiredExtensions;

        private:
            friend class VulkanInstance;

            VulkanPhysicalDevice(vk::PhysicalDevice& handle, const VulkanSurface& surface);

            void checkExtensions();
            void chooseQueueFamily(const VulkanSurface& surface);
            void rateDevice(const vk::PhysicalDeviceProperties& properties);

            bool usable_;
            vk::PhysicalDevice handle_;
            unsigned usedQueueFamily_;
            unsigned rating_;
            std::string name_;
    };
}
