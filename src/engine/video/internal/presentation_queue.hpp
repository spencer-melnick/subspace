#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace subspace {
    class RenderContext;
    class VulkanSwapchain;
    class VulkanPrimaryCommandBuffers;

    class PresentationQueue {
        public:
            PresentationQueue(const RenderContext& context, const VulkanSwapchain& swapchain,
                const VulkanPrimaryCommandBuffers& commands);
            ~PresentationQueue();

        private:
            struct FrameData {
                vk::UniqueSemaphore imageReady;
                vk::UniqueSemaphore renderDone;
                vk::UniqueFence fence;
                vk::CommandBuffer& command;
            };
            
            static const unsigned maxFrames;

            const VulkanSwapchain& swapchain_;

            unsigned activeFrame_ = 0;
            std::vector<FrameData> data_;
    };
}
