#include "render_context.hpp"

#include "wrappers/sdl_window.hpp"
#include "wrappers/vulkan_instance.hpp"
#include "wrappers/vulkan_physical_device.hpp"
#include "wrappers/vulkan_logical_device.hpp"

using namespace std;
using namespace subspace;

RenderContext::RenderContext() {
    instance_ = make_unique<VulkanInstance>();
    physicalDevice_ = &(instance_->getPhysicalDevices().begin()->second);
    logicalDevice_ = make_unique<VulkanLogicalDevice>(*physicalDevice_);
}

RenderContext::~RenderContext() = default;

const VulkanInstance& RenderContext::getInstance() const {
    return *instance_;
}

const VulkanPhysicalDevice& RenderContext::getPhysicalDevice() const {
    return *physicalDevice_;
}

const VulkanLogicalDevice& RenderContext::getLogicalDevice() const {
    return *logicalDevice_;
}
