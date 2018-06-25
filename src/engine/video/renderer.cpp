#include "renderer.hpp"

#include <vulkan/vulkan.hpp>

#include "../util/logger.hpp"
#include "render_context.hpp"
#include "wrappers/vulkan_pipeline.hpp"
#include "wrappers/vulkan_framebuffers.hpp"
#include "wrappers/vulkan_primary_command_buffers.hpp"

using namespace std;
using namespace subspace;

const unsigned Renderer::maxSynchronousFrames_ = 2;

Renderer::Renderer(const Window& window, const Shader& shader) :
    context_(window.getContext()), window_(window), shader_(shader),
    pipeline_(new VulkanPipeline(context_.getLogicalDevice(), window.getSwapchain(),
        shader_.getVertShader(), shader_.getFragShader())),
    framebuffers_(new VulkanFramebuffers(context_.getLogicalDevice(), window_.getSwapchain(),
        pipeline_->getRenderPass())),
    primaryCommands_(new VulkanPrimaryCommandBuffers(context_, window.getSwapchain(), *pipeline_,
        *framebuffers_))
{
    logger.logDebug("Created new renderer");
}

Renderer::~Renderer() = default;
