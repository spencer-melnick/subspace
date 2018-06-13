#include "render_context.hpp"

#include <SDL2/SDL_vulkan.h>

#include "../util/logger.hpp"
#include "video_exception.hpp"

using namespace std;

namespace subspace {
    RenderContext::RenderContext() {
        logger.logDebug("Creating dummy window for extension query");
        SDL_Window* dummyWindow = SDL_CreateWindow(
            "subspace",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            1, 1,
            SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN
        );

        if (dummyWindow == nullptr) {
            throw VideoException(VideoException::Type::WindowCreateFailure);
        }

        vulkanInstance_ = createVulkanInstance(dummyWindow);
        VkSurfaceKHR surfaceRaw;

        logger.logDebug("Creating dummy surface for extension query");
        if (SDL_Vulkan_CreateSurface(dummyWindow, vulkanInstance_, &surfaceRaw) != SDL_TRUE) {
            throw VideoException(VideoException::Type::SurfaceCreateFailure);
        }

        vk::SurfaceKHR dummySurface(surfaceRaw);

        auto devices = getSupportedDevices(dummySurface);
        auto& physicalDevice = (--devices.end())->second;

        vulkanInstance_.destroySurfaceKHR(dummySurface);
        SDL_DestroyWindow(dummyWindow);
        logger.logDebug("Freeing dummy window and surface");

        logger.logInfo("Using device - {}", physicalDevice.name);
        device_ = createLogicalDevice(physicalDevice);
    }

    RenderContext::~RenderContext() {
        device_.destroy();
        logger.logDebug("Destroyed Vulkan logical device");

        vulkanInstance_.destroy();
        logger.logDebug("Destroyed Vulkan instance");
    }

    RenderContext::DeviceList_ RenderContext::getSupportedDevices(vk::SurfaceKHR& surface) {
        DeviceList_ result;
        auto devices = vulkanInstance_.enumeratePhysicalDevices();

        if (devices.empty()) {
            throw VideoException(VideoException::Type::NoVulkanDevices);
        }
        logger.logVerbose("Found {} supported Vulkan devices:", devices.size());

        for (auto& i : devices) {
            DeviceWrapper_ device;
            device.vulkanDevice = i;

            auto deviceProperties = device.vulkanDevice.getProperties();
            device.name = string(deviceProperties.deviceName);

            if (findQueueFamily(device, surface)) {
                result.insert({rateDeviceSuitability(device), device});
            }
        }

        if (result.empty()) {
            throw VideoException(VideoException::Type::NoSupportedDevices);
        }

        return result;
    }

    vk::Instance RenderContext::createVulkanInstance(SDL_Window* window) {
        vk::ApplicationInfo appInfo(
            "Subspace",
            1,
            nullptr,
            0,
            VK_MAKE_VERSION(1, 0, 0)
        );

        vector<const char*> requiredExtensions = getRequiredExtensions(window);

        vk::InstanceCreateInfo instanceInfo(
            {},
            &appInfo,
            0,
            nullptr,
            requiredExtensions.size(),
            requiredExtensions.data()
        );

        vk::Instance instance;

        if (vk::createInstance(&instanceInfo, nullptr, &instance) != vk::Result::eSuccess) {
            throw VideoException(VideoException::Type::InstanceCreateFailure);
        }

        logger.logDebug("Created Vulkan instance");

        return instance;
    }

    vector<const char*> RenderContext::getRequiredExtensions(SDL_Window* window) {
        unsigned int numExtensions;
        vector<const char*> result;
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

    int RenderContext::rateDeviceSuitability(DeviceWrapper_& physicalDevice) {
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

    vk::Device RenderContext::createLogicalDevice(DeviceWrapper_& physicalDevice) {
        float queuePriority = 1.0f;
        
        vk::DeviceQueueCreateInfo queueInfo(
            {},
            physicalDevice.usedQueueFamily,
            1,
            &queuePriority
        );

        vk::DeviceCreateInfo deviceInfo(
            {},
            1,
            &queueInfo
        );

        return physicalDevice.vulkanDevice.createDevice(deviceInfo);
    }

    // NOTE: Currently doesn't check to see if the queue supports a specific Vulkan surface;
    // generally there is only one queue family that supports graphics, and this should be
    // the same one. Check here later if there are errors!
    bool RenderContext::findQueueFamily(DeviceWrapper_& device, vk::SurfaceKHR& surface) {
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
        
        logger.logWarning("{} is not compatible", device.name);
        return false;
    }
}
