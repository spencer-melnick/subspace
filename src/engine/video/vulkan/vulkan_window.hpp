#pragma once

// STL includes
#include <vector>

// Library includes
#include <vulkan/vulkan.hpp>

// Project includes 
#include "engine/util/config.hpp"
#include "../window.hpp"
#include "vulkan_renderer.hpp"
#include "sdl_window.hpp"
#include "swapchain.hpp"

namespace subspace {
    class VulkanWindow: public IWindow {
        public:
            VulkanWindow(const VulkanRenderer& renderer, const std::string& name,
                const Config& config);

            // Nested types
			struct Frame {
				vk::UniqueSemaphore available, drawn;
				vk::UniqueFence fence;
				vk::UniqueCommandBuffer presentBuffer;
			};

            // Interface functions
            virtual void swap() override;

            // Constants
            static const unsigned maxFramesInFlight = 2;

        private:
            // Initialization helpers
            void createFrames(const vk::Device& device);

            // Owner reference
            const VulkanRenderer& renderer_;

            // Member variables
            SdlWindow window_;
            vk::UniqueSurfaceKHR surface_;
            vk::UniqueCommandPool commandPool_;
            std::vector<Frame> frames_;
            unsigned currentFrame_ = 0;
            Swapchain swapchain_;
    };
}
