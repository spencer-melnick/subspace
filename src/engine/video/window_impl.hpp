#pragma once

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
        SDL_Window* sdlWindow_;
        RenderContext::Impl_& context_;
        vk::SurfaceKHR vulkanSurface_;

        static SDL_Window* createSdlWindow(const char* name, const Config& config);
        static vk::SurfaceKHR createVulkanSurface(SDL_Window* window, vk::Instance& instance);
};
