#include "render_context_impl.hpp"

#include <cstring>

#include "../util/logger.hpp"

using namespace std;
using namespace subspace;

RenderContext::Impl_::Impl_() {
    SDL_Window* dummyWindow = createDummyWindow();
    instance_ = createVulkanInstance(dummyWindow);
    vk::SurfaceKHR dummySurface = createDummySurface(dummyWindow, instance_);

    auto devices = getSupportedDevices(dummySurface);
    auto& physicalDevice = (--devices.end())->second;

    instance_.destroySurfaceKHR(dummySurface);
    SDL_DestroyWindow(dummyWindow);
    logger.logDebug("Freeing dummy window and surface");

    logger.logInfo("Using device - {}", physicalDevice.name);
    device_ = createLogicalDevice(physicalDevice);
}


RenderContext::Impl_::~Impl_() {
    device_.destroy();
    logger.logDebug("Destroyed Vulkan logical device");

    instance_.destroy();
    logger.logDebug("Destroyed Vulkan instance");
}


SDL_Window* RenderContext::Impl_::createDummyWindow() {
    const auto windowPos = SDL_WINDOWPOS_UNDEFINED;
    const auto windowFlags = SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN;

    logger.logDebug("Creating dummy window for extension query");
    SDL_Window* dummyWindow = SDL_CreateWindow("subspace", windowPos, windowPos, 1, 1, windowFlags);
    if (dummyWindow == nullptr) {
        throw VideoException(VideoException::Type::WindowCreateFailure);
    }

    return dummyWindow;
}


vk::SurfaceKHR RenderContext::Impl_::createDummySurface(SDL_Window* window, vk::Instance& instance) {
    logger.logVerbose("Creating dummy surface for extension query");
    VkSurfaceKHR surfaceRaw;
    if (SDL_Vulkan_CreateSurface(window, instance, &surfaceRaw) != SDL_TRUE) {
        throw VideoException(VideoException::Type::SurfaceCreateFailure);
    }

    vk::SurfaceKHR dummySurface(surfaceRaw);
    return dummySurface;
}


RenderContext::Impl_::DeviceList_ RenderContext::Impl_::getSupportedDevices(vk::SurfaceKHR& surface) {
    DeviceList_ result;
    auto devices = instance_.enumeratePhysicalDevices();

    if (devices.empty()) {
        throw VideoException(VideoException::Type::NoVulkanDevices);
    }
    logger.logVerbose("Found {} supported Vulkan devices:", devices.size());

    for (auto& i : devices) {
        DeviceWrapper_ device;
        device.vulkanDevice = i;

        auto deviceProperties = device.vulkanDevice.getProperties();
        device.name = string(deviceProperties.deviceName);

        if (findQueueFamily(device, surface) && checkDeviceExtensions(device.vulkanDevice)) {
            result.insert({rateDeviceSuitability(device), device});
        }
    }

    if (result.empty()) {
        throw VideoException(VideoException::Type::NoSupportedDevices);
    }

    return result;
}


vk::Instance RenderContext::Impl_::createVulkanInstance(SDL_Window* window) {
    vk::ApplicationInfo appInfo("Subspace", 1, nullptr, 0, VK_MAKE_VERSION(1, 0, 0));
    vector<const char*> extensions = getRequiredWindowExtensions(window);

    vk::InstanceCreateInfo instanceInfo({}, &appInfo, 0, nullptr, extensions.size(), extensions.data());
    vk::Instance instance;

    if (vk::createInstance(&instanceInfo, nullptr, &instance) != vk::Result::eSuccess) {
        throw VideoException(VideoException::Type::InstanceCreateFailure);
    }

    logger.logDebug("Created Vulkan instance");
    return instance;
}


vector<const char*> RenderContext::Impl_::getRequiredWindowExtensions(SDL_Window* window) {
    vector<const char*> result;
    unsigned numExtensions;
    const char** extensionList;

    if (!SDL_Vulkan_GetInstanceExtensions(window, &numExtensions, nullptr)) {
        throw VideoException(VideoException::Type::GetSurfaceExtensionsFailure);
    }

    extensionList = new const char* [numExtensions];
    if (!SDL_Vulkan_GetInstanceExtensions(window, &numExtensions, extensionList)) {
        throw VideoException(VideoException::Type::GetSurfaceExtensionsFailure);
    }

    logger.logVerbose("Required Vulkan extensions for window rendering:");
    for (unsigned int i = 0; i < numExtensions; i++) {
        result.push_back(extensionList[i]);
        logger.logVerbose(extensionList[i]);
    }

    delete[] extensionList;
    return result;
}


int RenderContext::Impl_::rateDeviceSuitability(DeviceWrapper_& physicalDevice) {
    int score = 0;
    auto properties = physicalDevice.vulkanDevice.getProperties();

    switch (properties.deviceType) {
        case vk::PhysicalDeviceType::eDiscreteGpu:
            score += 1000000;
            logger.logVerbose("Discrete GPU - {}", physicalDevice.name);
            break;
        case vk::PhysicalDeviceType::eIntegratedGpu:
            score += 50000;
            logger.logVerbose("Integrated GPU - {}", physicalDevice.name);
            break;
        default:
            logger.logVerbose("CPU or virtual device - {}", physicalDevice.name);
            break;
    }

    score += properties.limits.maxImageDimension2D;
    logger.logVerbose("Device score: {}", score);

    return score;
}


vk::Device RenderContext::Impl_::createLogicalDevice(DeviceWrapper_& physicalDevice) {
    float queuePriority = 1.0f;
    const auto& extensions = requiredDeviceExtensions_;
    
    vk::DeviceQueueCreateInfo queueInfo({}, physicalDevice.usedQueueFamily, 1, &queuePriority);
    vk::DeviceCreateInfo deviceInfo({}, 1, &queueInfo, 0, nullptr, extensions.size(), extensions.data());

    return physicalDevice.vulkanDevice.createDevice(deviceInfo);
}


bool RenderContext::Impl_::checkDeviceExtensions(vk::PhysicalDevice& device) {
    auto availableExtensions = device.enumerateDeviceExtensionProperties();
    bool foundExtension;

    for (auto i : requiredDeviceExtensions_) {
        foundExtension = false;

        for (auto j : availableExtensions) {
            if (strcmp(i, j.extensionName) == 0) {
                foundExtension = true;
                break;
            }
        }

        if (!foundExtension) {
            logger.logWarning("Device does not support extension {}", i);
            return false;            
        }
    }

    return true;
}


bool RenderContext::Impl_::findQueueFamily(DeviceWrapper_& device, vk::SurfaceKHR& surface) {
    const auto requriredFlags = vk::QueueFlagBits::eGraphics;
    auto queueProperties = device.vulkanDevice.getQueueFamilyProperties();

    for (unsigned i = 0; i < queueProperties.size(); i++) {
        auto family = queueProperties[i];

        bool flagSupport = (family.queueFlags & requriredFlags) == requriredFlags;
        bool presentSupport = device.vulkanDevice.getSurfaceSupportKHR(i, surface);

        if (family.queueCount > 0 && flagSupport && presentSupport) {
            device.usedQueueFamily = i;
            return true;
        }
    }
    
    logger.logWarning("{} does not have the required queue families", device.name);
    return false;
}

const vector<const char*> RenderContext::Impl_::requiredDeviceExtensions_ = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
