#include "vulkan_render_pass.hpp"

#include "../../util/logger.hpp"
#include "vulkan_logical_device.hpp"
#include "vulkan_swapchain.hpp"

using namespace subspace;

VulkanRenderPass::VulkanRenderPass(const VulkanLogicalDevice& device, const VulkanSwapchain& swapchain) :
    device_(device)
{
    vk::AttachmentDescription colorAttachment({}, swapchain.getFormat().format,
        vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);
        
    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 1, &colorAttachmentRef);

    vk::RenderPassCreateInfo createInfo({}, 1, &colorAttachment, 1, &subpass);
    handle_ = device_->createRenderPass(createInfo);
    logger.logDebug("Created render pass");
}

VulkanRenderPass::~VulkanRenderPass() {
    device_->destroyRenderPass(handle_);
    logger.logDebug("Destroyed render pass");
}

VulkanRenderPass::operator const vk::RenderPass&() const {
    return handle_;
}
