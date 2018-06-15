#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include "window.hpp"
#include "render_context_impl.hpp"

using namespace std;
using namespace subspace;

class Window::Impl_ {
    public:
        Impl_(RenderContext::Impl_& context, const std::string& name, const Config& config);
        ~Impl_();
        
    private:
        struct SwapchainWrapper_ {
            vk::SwapchainKHR swapchain;
            vk::SurfaceFormatKHR format;
            vk::PresentModeKHR mode;
            std::vector<vk::Image> images;
            std::vector<vk::ImageView> imageViews;
        };

        SDL_Window* sdlWindow_;
        RenderContext::Impl_& context_;
        vk::SurfaceKHR vulkanSurface_;
        SwapchainWrapper_ swapchain_;

        static SDL_Window* createSdlWindow(const char* name, const Config& config);

        vk::SurfaceKHR createVulkanSurface();
        vk::SurfaceFormatKHR chooseSurfaceFormat();
        vk::PresentModeKHR choosePresentMode();
        vk::Extent2D chooseSwapExtent();
        uint32_t chooseImageCount();
        SwapchainWrapper_ createSwapchain();
        std::vector<vk::ImageView> createImageViews(SwapchainWrapper_ swapchain);
};
