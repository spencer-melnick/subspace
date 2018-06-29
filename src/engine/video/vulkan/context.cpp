#include "context.hpp"

// STL includes
#include <algorithm>
#include <functional>

// Project includes 
#include "engine/util/logger.hpp"
#include "sdl_window.hpp"

using namespace std;
using namespace subspace;

Context::Context() {
    // Create temporary window to check required extensions
    SdlWindow tempWindow("Temp", 0, 1, 1, SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN);
    auto instanceExtensions = tempWindow.listRequiredExtensions();
    vector<const char*> instanceLayers;

    // In debug build enable validation layers
    #ifndef NDEBUG
        instanceExtensions.push_back("VK_EXT_debug_report");
        instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
    #endif

    // Create Vulkan instance
    vk::ApplicationInfo appInfo {"subspace", 1, nullptr, 0, VK_API_VERSION_1_0};
    instance_ = vk::createInstanceUnique(vk::InstanceCreateInfo{
        {}, &appInfo,
        static_cast<uint32_t>(instanceLayers.size()), instanceLayers.data(),
        static_cast<uint32_t>(instanceExtensions.size()), instanceExtensions.data()
    });

    #ifndef NDEBUG
        setupDebugLayers();
    #endif

    {
        vk::UniqueSurfaceKHR tempSurface {tempWindow.createSurface(*instance_), {*instance_}};
        enumerateDevices(*tempSurface);
        chosenDevice_ = &(*physicalDevices_.begin());
        choosePresentFormat(*tempSurface);
    }

    createLogicalDevice();
    mainQueue_ = logicalDevice_->getQueue(chosenDevice_->getUsedQueueFamily(), 0);
    logger.logVerbose("Created Vulkan rendering context");
}

const vk::Instance& Context::getInstance() const {
    return *instance_;
}

const vector<PhysicalDevice>& Context::listPhysicalDevices() const {
    return physicalDevices_;
}

const PhysicalDevice& Context::getChosenPhysicalDevice() const {
    return *chosenDevice_;
}

const vk::SurfaceFormatKHR& Context::getPresentFormat() const {
    return presentFormat_;
}

const vk::Device& Context::getLogicalDevice() const {
    return *logicalDevice_;
}

const vk::Queue& Context::getMainQueue() const {
    return mainQueue_;
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL Context::debugReportCallback(VkDebugReportFlagsEXT,
    VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t,
    const char*, const char* pMessage, void*)
{
    logger.logError("Vulkan error: {}", pMessage);

    return 0;
}

#ifndef NDEBUG
    static PFN_vkCreateDebugReportCallbackEXT pfn_vkCreateDebugReportCallbackEXT;
    static PFN_vkDebugReportMessageEXT pfn_vkDebugReportMessageEXT;
    static PFN_vkDestroyDebugReportCallbackEXT pfn_vkDestroyDebugReportCallbackEXT;
#endif

void Context::setupDebugLayers() {
    #ifndef NDEBUG
        // In debug build, load debug extensions
        pfn_vkCreateDebugReportCallbackEXT =
            reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>
                (vkGetInstanceProcAddr(*instance_, "vkCreateDebugReportCallbackEXT"));
        pfn_vkDebugReportMessageEXT =
            reinterpret_cast<PFN_vkDebugReportMessageEXT>
                (vkGetInstanceProcAddr(*instance_, "vkDebugReportMessageEXT"));
        pfn_vkDestroyDebugReportCallbackEXT =
            reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>
                (vkGetInstanceProcAddr(*instance_, "vkDestroyDebugReportCallbackEXT"));
        debugCallback_ = move(instance_->createDebugReportCallbackEXTUnique({{}, debugReportCallback}));
    #endif
}

void Context::enumerateDevices(const vk::SurfaceKHR& surface) {
    auto tempDevices = instance_->enumeratePhysicalDevices();

    for (auto& i : tempDevices) {
        physicalDevices_.emplace_back(i, surface);
    }

    if (physicalDevices_.empty()) {
        throw runtime_error("No suitable physical devices found");
    }

    sort(physicalDevices_.begin(), physicalDevices_.end(),
        [](const PhysicalDevice& a, const PhysicalDevice& b) {
            return a.getRating() > b.getRating();
        }
    );
}

void Context::choosePresentFormat(const vk::SurfaceKHR& surface) {
    auto availableFormats = (*chosenDevice_)->getSurfaceFormatsKHR(surface);

	// Special return value that means any format is allowed
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
		presentFormat_ = vk::SurfaceFormatKHR{vk::Format::eB8G8R8A8Snorm, vk::ColorSpaceKHR::eSrgbNonlinear};
		return;
	}

	for (const auto& i : availableFormats) {
		if (i.format == vk::Format::eB8G8R8A8Snorm &&
			i.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			presentFormat_ = i;
			return;
		}
	}

	presentFormat_ = availableFormats[0];
}

void Context::createLogicalDevice() {
    float queuePriority = 1.0f;
    vk::DeviceQueueCreateInfo queueInfo {{}, chosenDevice_->getUsedQueueFamily(), 1,
        &queuePriority};

    logicalDevice_ = (*chosenDevice_)->createDeviceUnique({
        {}, 1, &queueInfo, 0, nullptr,
        static_cast<uint32_t>(PhysicalDevice::requiredExtensions.size()),
        PhysicalDevice::requiredExtensions.data(), {}});
}

// Defining debug extension functions
#ifndef NDEBUG
    VkResult vkCreateDebugReportCallbackEXT(
        VkInstance                                  instance,
        const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDebugReportCallbackEXT*                   pCallback)
    {
        return pfn_vkCreateDebugReportCallbackEXT(
            instance,
            pCreateInfo,
            pAllocator,
            pCallback
        );
    }

    void vkDestroyDebugReportCallbackEXT(
        VkInstance                                  instance,
        VkDebugReportCallbackEXT                    callback,
        const VkAllocationCallbacks*                pAllocator)
    {
        pfn_vkDestroyDebugReportCallbackEXT(
            instance,
            callback,
            pAllocator
        );
    }

    void vkDebugReportMessageEXT(
        VkInstance                                  instance,
        VkDebugReportFlagsEXT                       flags,
        VkDebugReportObjectTypeEXT                  objectType,
        uint64_t                                    object,
        size_t                                      location,
        int32_t                                     messageCode,
        const char*                                 pLayerPrefix,
        const char*                                 pMessage)
    {
        pfn_vkDebugReportMessageEXT(
            instance,
            flags,
            objectType,
            object,
            location,
            messageCode,
            pLayerPrefix,
            pMessage
        );
    }
#endif
