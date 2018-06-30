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
            const vk::Instance& getInstance() const;
            const std::vector<PhysicalDevice>& listPhysicalDevices() const;
            const PhysicalDevice& getChosenPhysicalDevice() const;
            const vk::SurfaceFormatKHR& getPresentFormat() const;
            const vk::Device& getLogicalDevice() const;
            const vk::Queue& getMainQueue() const;

        private:
            // Callback functions
            static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugReportCallback(VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT  objectType,
                uint64_t object,
                size_t location,
                int32_t messageCode,
                const char* pLayerPrefix,
                const char* pMessage,
                void* pUserData);

            // Initialization helpers
            void setupDebugLayers();
            void enumerateDevices(const vk::SurfaceKHR& surface);
            void choosePresentFormat(const vk::SurfaceKHR& surface);
            void createLogicalDevice();

            // Member variables
            vk::UniqueInstance instance_;
            std::vector<PhysicalDevice> physicalDevices_;
            PhysicalDevice* chosenDevice_ = nullptr;
            vk::SurfaceFormatKHR presentFormat_;
            vk::UniqueDevice logicalDevice_;
            vk::Queue mainQueue_;

            // Debug variables
            #ifndef NDEBUG
                vk::UniqueDebugReportCallbackEXT debugCallback_;
            #endif
    };
}
