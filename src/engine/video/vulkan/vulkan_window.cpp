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
        window_(name, 0, config.getXResolution(), config.getYResolution(), SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE),
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

VulkanWindow::~VulkanWindow() {
    // Wait until device is idle before dumping resources
    renderer_.getContext().getLogicalDevice().waitIdle();
    logger.logVerbose("Destroyed Vulkan window");
}

void VulkanWindow::swap() {
    auto& device = renderer_.getContext().getLogicalDevice();
    auto& frame = frames_[currentFrame_];
    auto& extent = swapchain_.getExtent();

    // Get next framebuffer from swapchain
    unsigned frameIndex;
    auto result = device.acquireNextImageKHR(swapchain_, numeric_limits<uint64_t>::max(),
        *(frame.available), vk::Fence(nullptr), &frameIndex);

    if (result == vk::Result::eSuboptimalKHR || result == vk::Result::eErrorOutOfDateKHR) {
        recreateSwapchain();
        return;
    } else if (result == vk::Result::eNotReady) {
        logger.logWarning("Swapchain image not ready");
        return;
    } else if (result != vk::Result::eSuccess) {
        throw runtime_error("Failed to acquire image from swapchain");
    }

    auto& framebuffer = swapchain_.getFramebuffer(frameIndex);

    // Wait until command buffer is free
    device.waitForFences(1, &(*frame.fence), true, numeric_limits<uint64_t>::max());
    device.resetFences(1, &(*frame.fence));

    // Reset command buffer and record commands
    frame.presentBuffer->reset({});
    frame.presentBuffer->begin({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
    
        vk::ClearValue clearValue{{array<float, 4>{0.2f, 0.2f, 0.2f, 1.0f}}};

        frame.presentBuffer->beginRenderPass({renderer_.getRenderPass(), framebuffer,
            {{}, extent}, 1, &clearValue}, vk::SubpassContents::eInline);

            frame.presentBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics,
                renderer_.getPipeline());

            // Set dynamic viewport and scissor
            frame.presentBuffer->setViewport(0, {vk::Viewport{0.0f, 0.0f,
                static_cast<float>(extent.width), static_cast<float>(extent.height),
                0.0f, 1.0f}});
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
    renderer_.getContext().getMainQueue().submit({submitInfo}, *frame.fence);

    // Present image
    vk::Result presentResult = renderer_.getContext().getMainQueue().presentKHR(vk::PresentInfoKHR{
        1, &(*frame.drawn), 1, &static_cast<const vk::SwapchainKHR&>(swapchain_), &frameIndex
    });

    if (presentResult == vk::Result::eSuboptimalKHR || presentResult == vk::Result::eErrorOutOfDateKHR) {
        recreateSwapchain();
        return;
    } else if (presentResult != vk::Result::eSuccess) {
        throw runtime_error("Failed to present swapchain image");
    }

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

const SdlWindow& VulkanWindow::getSdlWindow() const {
    return window_;
}

void VulkanWindow::recreateSwapchain() {
    renderer_.getContext().getLogicalDevice().waitIdle();
    swapchain_ = move(Swapchain(renderer_.getContext(), window_,
        *surface_, renderer_.getRenderPass(), swapchain_));
}
