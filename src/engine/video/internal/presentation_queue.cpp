#include "presentation_queue.hpp"

using namespace std;
using namespace subspace;

#include "../render_context.hpp"
#include "../wrappers/vulkan_swapchain.hpp"
#include "../wrappers/vulkan_primary_command_buffers.hpp"

PresentationQueue::PresentationQueue(const RenderContext& context, const VulkanSwapchain& swapchain,
    const VulkanPrimaryCommandBuffers& commands) :
    swapchain_(swapchain)
{
    auto& device = context.getLogicalDevice();
    
    for (unsigned i = 0; i < maxFrames; i++) {
        data_.emplace_back(device->createUniqueSemaphore(),
            device->createUniqueSemaphore(),
            device->createUniqueFence({vk::FenceCreateFlagBits::eSignaled}));
    }
}

const unsigned PresentationQueue::maxFrames = 2;
