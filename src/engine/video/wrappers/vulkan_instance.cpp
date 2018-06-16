#include "vulkan_instance.hpp"

#include "../video_exception.hpp"
#include "../../util/logger.hpp"
#include "sdl_window.hpp"
#include "vulkan_surface.hpp"

using namespace std;
using namespace subspace;

VulkanInstance::VulkanInstance() {
    vk::ApplicationInfo appInfo("Subspace", 1, nullptr, 0, VK_MAKE_VERSION(1, 0, 0));
    SdlWindow dummyWindow("subspace", 0, 1, 1, SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN);
    auto extensions = dummyWindow.listRequiredExtensions();

    vk::InstanceCreateInfo instanceInfo({}, &appInfo, 0, nullptr,
        static_cast<uint32_t>(extensions.size()), extensions.data());

    if (vk::createInstance(&instanceInfo, nullptr, &handle_) != vk::Result::eSuccess) {
        throw VideoException(VideoException::Type::InstanceCreateFailure);
    }

    queryPhysicalDevices(dummyWindow);
    
    logger.logDebug("Created Vulkan instance");
}

VulkanInstance::~VulkanInstance() {
    handle_.destroy();
    logger.logDebug("Destroyed Vulkan instance");
}

const vk::Instance& VulkanInstance::getHandle() const {
    return handle_;
}

const VulkanInstance::PhysicalDeviceList& VulkanInstance::getPhysicalDevices() const {
    return physicalDevices_;
}

void VulkanInstance::queryPhysicalDevices(const SdlWindow& dummyWindow) {
    VulkanSurface dummySurface(handle_, dummyWindow.getHandle());

    auto physicalDevices = handle_.enumeratePhysicalDevices();

    for (auto& i : physicalDevices) {
        VulkanPhysicalDevice device(i, dummySurface.getHandle());

        if (device.isUsable()) {
            physicalDevices_.insert({device.getRating(), device});
        }
    }
}
