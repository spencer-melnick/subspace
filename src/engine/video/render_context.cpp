#include "render_context.hpp"

#include <SDL2/SDL_vulkan.h>
#include <fmt/format.h>

#include "../util/logger.hpp"
#include "video_exception.hpp"

using namespace std;

namespace subspace {
    RenderContext::RenderContext(const Window& window) {
        vulkanInstance_ = createVulkanInstance(window);

        auto devices = getSupportedDevices();
        auto& physicalDevice = (--devices.end())->second;

        logger.logInfo(fmt::format("Using device - {}", physicalDevice.name));
        device_ = createLogicalDevice(physicalDevice);
    }

    RenderContext::~RenderContext() {
        device_.destroy();
        logger.logDebug("Destroyed Vulkan logical device");

        vulkanInstance_.destroy();
        logger.logDebug("Destroyed Vulkan instance");
    }

    multimap<unsigned, RenderContext::PhysicalDeviceWrapper_> RenderContext::getSupportedDevices() {
        multimap<unsigned, PhysicalDeviceWrapper_> result;
        auto devices = vulkanInstance_.enumeratePhysicalDevices();

        if (devices.empty()) {
            throw VideoException(VideoException::Type::NoVulkanDevices);
        }
        logger.logVerbose(fmt::format("Found {} supported Vulkan devices:", devices.size()));

        for (auto& i : devices) {
            PhysicalDeviceWrapper_ device;
            device.vulkanDevice = i;

            auto deviceProperties = device.vulkanDevice.getProperties();
            device.name = string(deviceProperties.deviceName);

            if (findValidQueueFamily(device)) {
                result.insert({rateDeviceSuitability(device), device});
            }
        }

        if (result.empty()) {
            throw VideoException(VideoException::Type::NoSupportedDevices);
        }

        return result;
    }

    vk::Instance RenderContext::createVulkanInstance(const Window& window) {
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

    vector<const char*> RenderContext::getRequiredExtensions(const Window& window) {
        unsigned int numExtensions;
        vector<const char*> result;
        const char** extensionList;

        if (!SDL_Vulkan_GetInstanceExtensions(window.sdlWindow_, &numExtensions, nullptr)) {
            throw VideoException(VideoException::Type::GetSurfaceExtensionsFailure);
        }

        extensionList = new const char* [numExtensions];

        if (!SDL_Vulkan_GetInstanceExtensions(window.sdlWindow_, &numExtensions, extensionList)) {
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

    int RenderContext::rateDeviceSuitability(PhysicalDeviceWrapper_& physicalDevice) {
        int score = 0;
        auto properties = physicalDevice.vulkanDevice.getProperties();

        switch (properties.deviceType) {
            case vk::PhysicalDeviceType::eDiscreteGpu:
                score += 1000000;
                logger.logVerbose(fmt::format("Discrete GPU - {}", physicalDevice.name));
                break;
            case vk::PhysicalDeviceType::eIntegratedGpu:
                score += 50000;
                logger.logVerbose(fmt::format("Integrated GPU - {}", physicalDevice.name));
                break;
            default:
                logger.logVerbose(fmt::format("CPU or virtual device - {}", physicalDevice.name));
                break;
        }

        score += properties.limits.maxImageDimension2D;
        logger.logVerbose(fmt::format("Device score: {}", score));

        return score;
    }

    vk::Device RenderContext::createLogicalDevice(PhysicalDeviceWrapper_& physicalDevice) {
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

    bool RenderContext::findValidQueueFamily(PhysicalDeviceWrapper_& device) {
        auto queueProperties = device.vulkanDevice.getQueueFamilyProperties();

        for (unsigned i = 0; i < queueProperties.size(); i++) {
            auto family = queueProperties[i];

            if (family.queueCount > 0 && family.queueFlags & vk::QueueFlagBits::eGraphics) {
                device.usedQueueFamily = i;
                return true;
            }
        }
        
        logger.logWarning(fmt::format("{} is not compatible", device.name));
        return false;
    }
}
