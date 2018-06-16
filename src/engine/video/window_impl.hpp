#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include "window.hpp"
#include "render_context_impl.hpp"
#include "swapchain.hpp"

using namespace std;
using namespace subspace;

class Window::Impl {
    public:
        Impl(RenderContext& context, const std::string& name, const Config& config);
        ~Impl();
        
    private:

        SDL_Window* sdlWindow_;
        const RenderContext::InstanceHandle& instance_;
        const RenderContext::DeviceHandle& device_;
        vk::SurfaceKHR vulkanSurface_;
        SwapChain swapchain_;

        static SDL_Window* createSdlWindow(const char* name, const Config& config);

        vk::SurfaceKHR createVulkanSurface();
};
