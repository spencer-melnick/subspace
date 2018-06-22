#include "renderer.hpp"

#include "../util/logger.hpp"
#include "wrappers/vulkan_pipeline.hpp"

using namespace std;
using namespace subspace;

Renderer::Renderer(const Window& window, const Shader& shader) :
    context_(window.getContext()), window_(window), shader_(shader),
    pipeline_(new VulkanPipeline(context_.getLogicalDevice(), window.getSwapchain(),
        shader_.getVertShader(), shader_.getFragShader()))
{
    logger.logDebug("Created new renderer");
}

Renderer::~Renderer() = default;
