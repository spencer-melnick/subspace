#pragma once

// Library includes
#include <vulkan/vulkan.hpp>

// Project includes 
#include "../window.hpp"
#include "sdl_window.hpp"

namespace subspace {
    class VulkanWindow: public IWindow {
        public:
            virtual void swap() override;

        private:
            SdlWindow window_;
            vk::SurfaceKHR surface_;

            void createSurface();
    };
}
