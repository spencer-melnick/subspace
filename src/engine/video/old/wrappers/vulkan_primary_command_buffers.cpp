#include "vulkan_primary_command_buffers.hpp"

#include "../../util/logger.hpp"
#include "../render_context.hpp"
#include "vulkan_logical_device.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_swapchain.hpp"
#include "vulkan_pipeline.hpp"
#include "vulkan_framebuffers.hpp"

using namespace std;
using namespace subspace;

VulkanPrimaryCommandBuffers::VulkanPrimaryCommandBuffers(const RenderContext& context,
    const VulkanSwapchain& swapchain, const VulkanPipeline& pipeline,
    const VulkanFramebuffers& framebuffers) :
    device_(context.getLogicalDevice())
{
    vk::CommandPoolCreateInfo poolInfo({}, context.getPhysicalDevice().getUsedQueueFamily());
    pool_ = context.getLogicalDevice()->createCommandPool(poolInfo);

    handles_.resize(swapchain.getImageViews().size());
    vk::CommandBufferAllocateInfo createInfo(pool_,
        vk::CommandBufferLevel::ePrimary, handles_.size());

    device_->allocateCommandBuffers(&createInfo, handles_.data());
    logger.logDebug("Allocated command buffers");

    recordCommands(swapchain, pipeline, framebuffers);
}

VulkanPrimaryCommandBuffers::~VulkanPrimaryCommandBuffers() {
    device_->destroyCommandPool(pool_);
    logger.logDebug("Destroyed command pool");
}

void VulkanPrimaryCommandBuffers::recordCommands(const VulkanSwapchain& swapchain,
    const VulkanPipeline& pipeline, const VulkanFramebuffers& framebuffers)
{
    auto buffers = static_cast<const vector<vk::Framebuffer>&>(framebuffers);
    vk::Rect2D area({0, 0}, swapchain.getExtent());

    for (unsigned i = 0; i < handles_.size(); i++) {
        vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
        handles_[i].begin(&beginInfo);

        vk::ClearColorValue clearColor(array<int32_t, 4>{0, 0, 0, 0});
        vk::ClearValue clearValue(clearColor);

        vk::RenderPassBeginInfo renderInfo(pipeline.getRenderPass(), buffers[i],
            area, 1, &clearValue);
        handles_[i].beginRenderPass(renderInfo, vk::SubpassContents::eInline);
        handles_[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
        handles_[i].draw(3, 1, 0, 0);

        handles_[i].endRenderPass();
        handles_[i].end();
    }

    logger.logDebug("Recorded primary command buffers");
}
