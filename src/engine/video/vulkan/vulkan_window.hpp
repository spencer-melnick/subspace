#pragma once

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

            virtual void swap() override;

        private:
            SdlWindow window_;
            vk::UniqueSurfaceKHR surface_;
            Swapchain swapchain_;
    };
}
