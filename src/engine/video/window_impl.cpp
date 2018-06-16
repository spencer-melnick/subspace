#include "window_impl.hpp"

#include "../util/logger.hpp"

using namespace std;
using namespace subspace;

Window::Impl_::Impl_(RenderContext::Impl_& context, const std::string& name, const Config& config) :
    context_(context)
{
    sdlWindow_ = createSdlWindow(name.c_str(), config);
    vulkanSurface_ = createVulkanSurface();
    swapchain_ = SwapChain::SwapChain(vulkanSurface_, context_);
}

Window::Impl_::~Impl_() {
    if (context_.instance_) {
        for (auto& i : swapchain_.imageViews) {
            context_.device_.destroyImageView(i);
        }
        logger.logDebug("Destroyed image views");

        context_.device_.destroySwapchainKHR(swapchain_.swapchain);
        logger.logDebug("Destroyed window swapchain");
        
        context_.instance_.destroySurfaceKHR(vulkanSurface_);
        logger.logDebug("Destroyed Vulkan surface");
    } else {
        logger.logWarning("Could not destroy Vulkan surface; instance already destroyed");
    }

    SDL_DestroyWindow(sdlWindow_);
    logger.logDebug("Destroyed SDL2 window");
}

SDL_Window* Window::Impl_::createSdlWindow(const char* name, const Config& config) {
    Uint32 flags = SDL_WINDOW_VULKAN;

    if (config.isFullscreen()) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    SDL_Window* window = SDL_CreateWindow(
        name,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        config.getXResolution(), config.getYResolution(),
        flags
    );

    if (window == nullptr) {
        logger.logError("SDL2 error: {}", SDL_GetError());
        throw VideoException(VideoException::Type::WindowCreateFailure);
    }
    logger.logDebug("Created SDL2 window {}", name);

    return window;
}

vk::SurfaceKHR Window::Impl_::createVulkanSurface() {
    VkSurfaceKHR surfaceRaw;
    if (SDL_Vulkan_CreateSurface(sdlWindow_, context_.instance_, &surfaceRaw) != SDL_TRUE) {
        throw VideoException(VideoException::Type::SurfaceCreateFailure);
    }
    logger.logDebug("Created Vulkan surface for window");

    return vk::SurfaceKHR(surfaceRaw);
}

vk::SurfaceFormatKHR Window::Impl_::chooseSurfaceFormat() {
    auto& physicalDevice = context_.physicalDevice_.vulkanDevice;
    auto availableFormats = physicalDevice.getSurfaceFormatsKHR(vulkanSurface_);

    // Special return value that means any format is allowed
    if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
        return {vk::Format::eB8G8R8A8Snorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    }

    for (const auto& i : availableFormats) {
        if (i.format == vk::Format::eB8G8R8A8Snorm && i.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return i;
        }
    }

    return availableFormats[0];
}