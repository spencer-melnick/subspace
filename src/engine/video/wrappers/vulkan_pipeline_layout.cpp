#include "vulkan_pipeline_layout.hpp"

#include "../../util/logger.hpp"
#include "vulkan_logical_device.hpp"

using namespace subspace;

VulkanPipelineLayout::VulkanPipelineLayout(const VulkanLogicalDevice& device) :
    device_(device)
{
    vk::PipelineLayoutCreateInfo createInfo;

    handle_ = device_->createPipelineLayout(createInfo);
    logger.logDebug("Created pipeline layout");
}

VulkanPipelineLayout::~VulkanPipelineLayout() {
    device_->destroyPipelineLayout(handle_);
}

VulkanPipelineLayout::operator const vk::PipelineLayout&() const {
    return handle_;
}
