#pragma once

// STL includes
#include <vector>

// Library includes
#include <vulkan/vulkan.hpp>

// Project includes 
#include "physical_device.hpp"

namespace subspace {
    class Context {
        public: 
            Context();

            // Getters
            const vk::UniqueInstance& getInstance() const;
            const vk::PhysicalDevice& getPhysicalDevice() const;
            const vk::UniqueDevice& getLogicalDevice() const;

        private:
            // Initialization helpers
            void enumerateDevices(const vk::SurfaceKHR& surface);
            void createLogicalDevice();

            // Member variables
            vk::UniqueInstance instance_;
            std::vector<PhysicalDevice> physicalDevices_;
            PhysicalDevice* chosenDevice_ = nullptr;
            vk::UniqueDevice logicalDevice_;
    };
}
