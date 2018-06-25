#include "vulkan_framebuffers.hpp"

#include "../../util/logger.hpp"
#include "vulkan_logical_device.hpp"
#include "vulkan_swapchain.hpp"
#include "vulkan_render_pass.hpp"

using namespace std;
using namespace subspace;

VulkanFramebuffers::VulkanFramebuffers(const VulkanLogicalDevice& device,
    const VulkanSwapchain& swapchain, const VulkanRenderPass& renderPass) :
    device_(device), swapchain_(swapchain), renderPass_(renderPass)
{
    auto extent = swapchain_.getExtent();

    for (auto& i : swapchain_.getImageViews()) {
        vk::FramebufferCreateInfo createInfo({}, renderPass_, 1, &i,
            extent.width, extent.height, 1);
        handles_.push_back(device_->createFramebuffer(createInfo));
    }

    logger.logDebug("Created framebuffers");
}

VulkanFramebuffers::~VulkanFramebuffers() {
    for (auto& i : handles_) {
        device_->destroyFramebuffer(i);
    }

    logger.logDebug("Destroyed framebuffers");
}

VulkanFramebuffers::operator const vector<vk::Framebuffer>&() const {
    return handles_;
}
