#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Class that provides simple window functionality by wrapping the SDL2 window
 * interface.
 */

#include <string>
#include <memory>

#include "render_context.hpp"
#include "../util/config.hpp"

namespace subspace {
    class SdlWindow;
    class VulkanSurface;
    class VulkanSwapchain;

    class Window {
        /**
         * Class that provides simple window functionality by wrapping the SDL2 window
         * interface.
         */

        public:
            Window(const RenderContext& context, const std::string& name, const Config& config);
            ~Window();

            const RenderContext& getContext() const;
            const VulkanSwapchain& getSwapchain() const;

        private:
            std::unique_ptr<SdlWindow> sdlWindow_;
            std::unique_ptr<VulkanSurface> vulkanSurface_;
            std::unique_ptr<VulkanSwapchain> vulkanSwapchain_;

            const RenderContext& context_;
    };
}
