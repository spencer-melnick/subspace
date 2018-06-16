#include "vulkan_physical_device.hpp"

#include "../../util/logger.hpp"

using namespace std;
using namespace subspace;

const vector<const char*> VulkanPhysicalDevice::requiredExtensions {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VulkanPhysicalDevice::VulkanPhysicalDevice(vk::PhysicalDevice& handle, const vk::SurfaceKHR& surface) :
    usable_(true), handle_(handle)
{
    vk::PhysicalDeviceProperties properties = handle_.getProperties();
    name_ = properties.deviceName;

    checkExtensions();
    chooseQueueFamily(surface);
    rateDevice(properties);

    if (!usable_) {
        logger.logWarning("Device {} does not support the minimum requirements", name_);
    }
}

bool VulkanPhysicalDevice::isUsable() const {
    return usable_;
}

const vk::PhysicalDevice& VulkanPhysicalDevice::getHandle() const {
    return handle_;
}

unsigned VulkanPhysicalDevice::getUsedQueueFamily() const {
    return usedQueueFamily_;
}

unsigned VulkanPhysicalDevice::getRating() const {
    return rating_;
}

const std::string& VulkanPhysicalDevice::getName() const {
    return name_;
}

void VulkanPhysicalDevice::checkExtensions() {
    auto availableExtensions = handle_.enumerateDeviceExtensionProperties();
    bool foundExtension;

    for (auto i : requiredExtensions) {
        foundExtension = false;

        for (auto j : availableExtensions) {
            if (strcmp(i, j.extensionName) == 0) {
                foundExtension = true;
                break;
            }
        }

        if (!foundExtension) {
            logger.logWarning("Device {} does not support extension {}", name_, i);
            usable_ = false;          
        }
    }
}

void VulkanPhysicalDevice::chooseQueueFamily(const vk::SurfaceKHR& surface) {
    const auto requriredQueueFlags = vk::QueueFlagBits::eGraphics;

    bool foundQueueFamily = false;
    auto queueFamilyProperties = handle_.getQueueFamilyProperties();

    for (unsigned i = 0; i < queueFamilyProperties.size(); i++) {
        auto family = queueFamilyProperties[i];

        bool flagSupport = (family.queueFlags & requriredQueueFlags) == requriredQueueFlags;
        bool presentSupport = handle_.getSurfaceSupportKHR(i, surface);

        if (family.queueCount > 0 && flagSupport && presentSupport) {
            usedQueueFamily_ = i;
            foundQueueFamily = true;
            break;
        }
    }
    
    if (!foundQueueFamily) {
        usable_ = false;
        logger.logWarning("Device {} does not have the required queue families", name_);
    }
}

void VulkanPhysicalDevice::rateDevice(const vk::PhysicalDeviceProperties& properties) {
    switch (properties.deviceType) {
        case vk::PhysicalDeviceType::eDiscreteGpu:
            rating_ += 1000000;
            logger.logVerbose("Device {} - Discrete GPU", name_);
            break;
        case vk::PhysicalDeviceType::eIntegratedGpu:
            rating_ += 50000;
            logger.logVerbose("Device {} - Integrated GPU", name_);
            break;
        default:
            logger.logVerbose("Device {} - CPU or virtual device", name_);
            break;
    }

    auto memoryProperties = handle_.getMemoryProperties();
    unsigned memoryBytes = 0;

    for (unsigned i = 0; i < memoryProperties.memoryHeapCount; i++) {
        auto& heap = memoryProperties.memoryHeaps[i];

        if (heap.flags & vk::MemoryHeapFlagBits::eDeviceLocal) {
            memoryBytes += heap.size;
        }
    }
    logger.logVerbose("Device {} has {} bytes of memory", name_, memoryBytes);    

    rating_ += memoryBytes / (1024 * 1024);
    logger.logVerbose("Device {} suitability score: {}", name_, rating_);
}
