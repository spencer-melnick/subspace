#include "window.hpp"

#include "wrappers/sdl_window.hpp"
#include "wrappers/vulkan_instance.hpp"
#include "wrappers/vulkan_surface.hpp"
#include "wrappers/vulkan_swapchain.hpp"

using namespace std;
using namespace subspace;

Window::Window(const RenderContext& context, const string& name, const Config& config) {
    uint32_t flags = SDL_WINDOW_VULKAN;

    if (config.isFullscreen()) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    sdlWindow_ = make_unique<SdlWindow>(name, 0, config.getXResolution(), config.getYResolution(),
        flags);
    vulkanSurface_ = make_unique<VulkanSurface>(context.getInstance().getHandle(),
        sdlWindow_->getHandle());
    vulkanSwapchain_ = make_unique<VulkanSwapchain>(context, *sdlWindow_, *vulkanSurface_);
}

Window::~Window() = default;
