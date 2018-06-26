#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace subspace {
    class PhysicalDevice {
        public:        
            PhysicalDevice(vk::PhysicalDevice& handle, const vk::SurfaceKHR& surface);

            // Cast to underlying type
            operator const vk::PhysicalDevice&() const;
            const vk::PhysicalDevice* operator->() const;

            // Getters
            bool isUsable() const;
            unsigned getUsedQueueFamily() const;
            unsigned getRating() const;
            const std::string& getName() const;

            // Constants
            static const std::vector<const char*> requiredExtensions;

        private:
            // Initialization helpers
            void checkExtensions();
            void chooseQueueFamily(const vk::SurfaceKHR& surface);
            void rateDevice();

            // Member variables
            vk::PhysicalDevice handle_;
            bool usable_ = true;
            unsigned usedQueueFamily_, rating_;
            std::string name_;
    };
}
