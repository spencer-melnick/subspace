#include "context.hpp"

// STL includes
#include <algorithm>
#include <functional>

// Project includes 
#include "sdl_window.hpp"

using namespace std;
using namespace subspace;

Context::Context() {
    SdlWindow tempWindow("", 0, 1, 1, SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN);

    {
        auto instanceExtensions = tempWindow.listRequiredExtensions();

        vk::ApplicationInfo appInfo {"subspace", 1, nullptr, 0, VK_API_VERSION_1_0};
        instance_ = vk::createInstanceUnique({
            {}, &appInfo, 0, nullptr, static_cast<uint32_t>(instanceExtensions.size()), 
            instanceExtensions.data()
        });
    }

    vk::UniqueSurfaceKHR tempSurface {tempWindow.createSurface(*instance_)};
    enumerateDevices(*tempSurface);
    chosenDevice_ = &(*physicalDevices_.begin());

    createLogicalDevice();
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

void Context::createLogicalDevice() {
    float queuePriority = 1.0f;
    vk::DeviceQueueCreateInfo queueInfo {{}, chosenDevice_->getUsedQueueFamily(), 1,
        &queuePriority};

    logicalDevice_ = (*chosenDevice_)->createDeviceUnique({
        {}, 1, &queueInfo, 0, nullptr,
        static_cast<uint32_t>(PhysicalDevice::requiredExtensions.size()),
        PhysicalDevice::requiredExtensions.data(), {}});
}
