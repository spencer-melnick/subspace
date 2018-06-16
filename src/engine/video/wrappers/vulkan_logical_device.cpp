#include "vulkan_logical_device.hpp"

#include "../../util/logger.hpp"
#include "../video_exception.hpp"

using namespace subspace;

VulkanLogicalDevice::VulkanLogicalDevice(const VulkanPhysicalDevice& physicalDevice) {
    float queuePriority = 1.0f;
    const auto& extensions = VulkanPhysicalDevice::requiredExtensions;
    
    vk::DeviceQueueCreateInfo queueInfo({}, physicalDevice.getUsedQueueFamily(), 1, &queuePriority);
    vk::DeviceCreateInfo deviceInfo({}, 1, &queueInfo, 0, nullptr,
        static_cast<uint32_t>(extensions.size()), extensions.data());

    handle_ = physicalDevice.getHandle().createDevice(deviceInfo);
    logger.logDebug("Created logical device for {}", physicalDevice.getName());
}

VulkanLogicalDevice::~VulkanLogicalDevice() {
    handle_.destroy();
    logger.logDebug("Destroyed logical device");
}

const vk::Device& VulkanLogicalDevice::getHandle() const {
    return handle_;
}
