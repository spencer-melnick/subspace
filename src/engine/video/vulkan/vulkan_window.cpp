#include "vulkan_window.hpp"

// STL includes
#include <utility>
#include <array>
#include <limits>

// Project includes
#include "engine/util/logger.hpp"

using namespace std;
using namespace subspace;

VulkanWindow::VulkanWindow(const VulkanRenderer& renderer, const std::string& name,
    const Config& config) :
        renderer_(renderer),
        window_(name, 0, config.getXResolution(), config.getYResolution(), SDL_WINDOW_VULKAN),
        surface_(window_.createSurface(renderer.getContext().getInstance()), {
            renderer.getContext().getInstance()
        }),
        commandPool_(renderer.getContext().getLogicalDevice().createCommandPoolUnique({
            {vk::CommandPoolCreateFlagBits::eTransient |
                vk::CommandPoolCreateFlagBits::eResetCommandBuffer},
            renderer.getContext().getChosenPhysicalDevice().getUsedQueueFamily()
        })),
        swapchain_(renderer.getContext(), window_, *surface_, renderer.getRenderPass())
{
    createFrames(renderer.getContext().getLogicalDevice());
    logger.logVerbose("Created window");
}

void VulkanWindow::swap() {
    // TODO: Add check for out of date swapchain

    auto& device = renderer_.getContext().getLogicalDevice();
    auto& frame = frames_[currentFrame_];
    auto& extent = swapchain_.getExtent();

    // Wait until command buffer is free
    device.waitForFences(1, &(*frame.fence), true, numeric_limits<uint32_t>::max());
    device.resetFences(1, &(*frame.fence));

    // Get next framebuffer from swapchain
    auto result = device.acquireNextImageKHR(swapchain_, numeric_limits<uint32_t>::max(),
        *(frame.available), vk::Fence());
    auto& framebuffer = swapchain_.getFramebuffer(result.value);

    // Reset command buffer and record commands
    frame.presentBuffer->reset({});
    frame.presentBuffer->begin({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
        vk::ClearValue clearValue{{array<float, 4>{255.0f, 0.0f, 0.0f, 1.0f}}};
        frame.presentBuffer->beginRenderPass({renderer_.getRenderPass(), framebuffer,
            {{}, extent}, 1, &clearValue}, vk::SubpassContents::eInline);
            frame.presentBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics,
                renderer_.getPipeline());

            // Set dynamic viewport and scissor
            frame.presentBuffer->setViewport(0, {vk::Viewport{0.0f, 0.0f,
                static_cast<float>(extent.width), static_cast<float>(extent.height)}});
            frame.presentBuffer->setScissor(0, {vk::Rect2D{{}, extent}});
            
            // Actual draw commands would go here
            // Probably using secondary command buffer
            frame.presentBuffer->draw(3, 1, 0, 0);

        frame.presentBuffer->endRenderPass();
    frame.presentBuffer->end();

    // Submit commands
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::SubmitInfo submitInfo{1, &(*frame.available), waitStages,
        1, &(*frame.presentBuffer), 1, &(*frame.drawn)};
    renderer_.getContext().getMainQueue().submit({submitInfo}, {});

    // Present image
    renderer_.getContext().getMainQueue().presentKHR(vk::PresentInfoKHR{
        1, &(*frame.drawn), 1, &static_cast<const vk::SwapchainKHR&>(swapchain_), &result.value
    });

    // Increment the frame
    currentFrame_++;
    currentFrame_ %= maxFramesInFlight;
}

void VulkanWindow::createFrames(const vk::Device& device) {
    for (unsigned i = 0; i < maxFramesInFlight; i++) {
		Frame frame;

		frame.available = device.createSemaphoreUnique({});
		frame.drawn = device.createSemaphoreUnique({});
		frame.fence = device.createFenceUnique({vk::FenceCreateFlagBits::eSignaled});
		frame.presentBuffer = move(device.allocateCommandBuffersUnique({
			*commandPool_, vk::CommandBufferLevel::ePrimary, 1
		})[0]);

		frames_.push_back(move(frame));
	}
}
