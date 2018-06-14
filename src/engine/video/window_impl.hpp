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
        Impl_(RenderContext::Impl_& context, const std::string& name, const Config& config) ;
        ~Impl_();
        
    private:
        SDL_Window* sdlWindow_;
        vk::Instance& vulkanInstance_;
        vk::Device& vulkanDevice_;
        vk::SurfaceKHR vulkanSurface_;
};
