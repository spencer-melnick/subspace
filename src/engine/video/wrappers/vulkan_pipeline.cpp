#include "vulkan_pipeline.hpp"

#include "../../util/logger.hpp"
#include "vulkan_logical_device.hpp"
#include "vulkan_swapchain.hpp"
#include "vulkan_shader_module.hpp"

#include <vector>

using namespace std;
using namespace subspace;

VulkanPipeline::VulkanPipeline(const VulkanLogicalDevice& device, const VulkanSwapchain& swapchain,
    const VulkanShaderModule& vertShader, const VulkanShaderModule& fragShader) :
    device_(device), renderPass_(device, swapchain), layout_(device)
{
    vk::PipelineVertexInputStateCreateInfo vertextInputInfo;
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo({},
        vk::PrimitiveTopology::eTriangleList);

    vector<vk::PipelineShaderStageCreateInfo> shaderStages;
    shaderStages.push_back(vk::PipelineShaderStageCreateInfo({},
        vk::ShaderStageFlagBits::eVertex, vertShader, "main"));
    shaderStages.push_back(vk::PipelineShaderStageCreateInfo({},
        vk::ShaderStageFlagBits::eFragment, fragShader, "main"));

    auto extent = swapchain.getExtent();
    vk::Viewport viewport(0, 0, static_cast<float>(extent.width), static_cast<float>(extent.height),
        0.0f, 1.0f);
    vk::Rect2D scissor({}, extent);
    vk::PipelineViewportStateCreateInfo viewportInfo({}, 1, &viewport, 1, &scissor);

    vk::PipelineRasterizationStateCreateInfo rasterizerInfo({}, 0, 0, vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack, vk::FrontFace::eCounterClockwise, 0, 0, 0, 0, 1.0f);
    
    vk::PipelineMultisampleStateCreateInfo multisampleInfo({}, vk::SampleCountFlagBits::e1, 0,
        1.0f, nullptr, 0, 0);

    vk::PipelineColorBlendAttachmentState colorBlendAttachment(1, vk::BlendFactor::eSrcAlpha,
        vk::BlendFactor::eOneMinusDstAlpha, vk::BlendOp::eAdd, vk::BlendFactor::eOne,
        vk::BlendFactor::eZero, vk::BlendOp::eAdd);

    vk::PipelineColorBlendStateCreateInfo colorBlend({}, 0, vk::LogicOp::eCopy, 1,
        &colorBlendAttachment);

    vk::GraphicsPipelineCreateInfo createInfo({}, shaderStages.size(), shaderStages.data(),
        &vertextInputInfo, &inputAssemblyInfo, nullptr, &viewportInfo, &rasterizerInfo,
        &multisampleInfo, nullptr, &colorBlend, nullptr, layout_, renderPass_, 0, vk::Pipeline(),
        -1);

    handle_ = device_->createGraphicsPipeline({}, createInfo);
    logger.logDebug("Created rendering pipeline");
}

VulkanPipeline::~VulkanPipeline() {
    device_->destroyPipeline(handle_);
    logger.logDebug("Destroyed rendering pipeline");
}

VulkanPipeline::operator const vk::Pipeline&() const{
    return handle_;
}

const VulkanRenderPass& VulkanPipeline::getRenderPass() const {
    return renderPass_;
}
