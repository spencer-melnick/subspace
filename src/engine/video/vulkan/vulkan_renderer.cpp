#include "vulkan_renderer.hpp"

// STL includes
#include <utility>

// Project includes
#include "engine/util/logger.hpp"
#include "engine/util/simple_file.hpp"

using namespace std;
using namespace subspace;

VulkanRenderer::VulkanRenderer() {
    createRenderPass();
    createPipeline();
    logger.logVerbose("Created Vulkan renderer");
}

VulkanRenderer::~VulkanRenderer() {
    // Wait until device is idle before dumping all of our resources
    context_.getLogicalDevice().waitIdle();
    logger.logVerbose("Destroyed Vulkan renderer");
}

void VulkanRenderer::draw() {
    // Generate secondary command buffers!
}

const Context& VulkanRenderer::getContext() const {
    return context_;
}

const vk::RenderPass& VulkanRenderer::getRenderPass() const {
    return *renderPass_;
}

const vk::Pipeline& VulkanRenderer::getPipeline() const {
    return *pipeline_;
}

void VulkanRenderer::createRenderPass() {
    vk::AttachmentDescription colorAttachment({}, context_.getPresentFormat().format,
        vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);
        
    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr,
        1, &colorAttachmentRef);
    vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::AccessFlagBits::eColorAttachmentRead,
        vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);

    vk::RenderPassCreateInfo createInfo({}, 1, &colorAttachment, 1, &subpass, 1, &dependency);
    renderPass_ = context_.getLogicalDevice().createRenderPassUnique(createInfo);
}

void VulkanRenderer::createPipeline() {
    // Add shaders and pipeline layout
    auto& device = context_.getLogicalDevice();

    // Create pipeline layout with default constructor
    pipelineLayout_ = move(device.createPipelineLayoutUnique({}));

    vk::PipelineVertexInputStateCreateInfo vertextInputInfo;
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo({},
        vk::PrimitiveTopology::eTriangleList);
    
    // TODO: Add reconfigurable shaders
    // Load example shaders for now
    string vertSource = simpleFileRead("rc/shaders/test/vert.spv");
    string fragSource = simpleFileRead("rc/shaders/test/frag.spv");

    // Send shaders to GPU
    vk::UniqueShaderModule vertextShader = device.createShaderModuleUnique({
        {}, vertSource.size(), reinterpret_cast<const uint32_t*>(vertSource.data())
    });
    vk::UniqueShaderModule fragmentShader = device.createShaderModuleUnique({
        {}, fragSource.size(), reinterpret_cast<const uint32_t*>(fragSource.data())
    });

    // Fragment and vertex shader stages
    vector<vk::PipelineShaderStageCreateInfo> shaderStages;
    shaderStages.push_back(vk::PipelineShaderStageCreateInfo({},
        vk::ShaderStageFlagBits::eVertex, *vertextShader, "main"));
    shaderStages.push_back(vk::PipelineShaderStageCreateInfo({},
        vk::ShaderStageFlagBits::eFragment, *fragmentShader, "main"));

    vk::Viewport viewport{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    vk::Rect2D scissor;
    vk::PipelineViewportStateCreateInfo viewportInfo{{}, 1, &viewport, 1, &scissor};

    // Polygon fill, cull counter clockwise faces
    vk::PipelineRasterizationStateCreateInfo rasterizerInfo({}, 0, 0, vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise, 0, 0, 0, 0, 1.0f);
    
    // Only using 1 sample per pixel
    vk::PipelineMultisampleStateCreateInfo multisampleInfo({}, vk::SampleCountFlagBits::e1, 0,
        1.0f, nullptr, 0, 0);

    vk::PipelineColorBlendAttachmentState colorBlendAttachment(1, vk::BlendFactor::eSrcAlpha,
        vk::BlendFactor::eOneMinusDstAlpha, vk::BlendOp::eAdd, vk::BlendFactor::eOne,
        vk::BlendFactor::eZero, vk::BlendOp::eAdd);

    vk::PipelineColorBlendStateCreateInfo colorBlend({}, 0, vk::LogicOp::eCopy, 1,
        &colorBlendAttachment);

    // Create pipeline with dynamic viewport and scissor so we don't have to recreate
    // the pipeline every time the swapchain changes
    vk::DynamicState pipelineStates[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    vk::PipelineDynamicStateCreateInfo dynamicState{{}, 2, pipelineStates};

    vk::GraphicsPipelineCreateInfo createInfo({}, shaderStages.size(), shaderStages.data(),
        &vertextInputInfo, &inputAssemblyInfo, nullptr, &viewportInfo, &rasterizerInfo,
        &multisampleInfo, nullptr, &colorBlend, &dynamicState, *pipelineLayout_,
        *renderPass_, 0, vk::Pipeline(), -1);

    // Move assign to avoid unwanted deallocation
    pipeline_ = move(device.createGraphicsPipelineUnique({}, createInfo));
}
