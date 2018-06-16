#include "window_impl.hpp"

#include "../util/logger.hpp"

using namespace std;
using namespace subspace;

Window::Impl::Impl(RenderContext& context, const std::string& name, const Config& config) :
    sdlWindow_(createSdlWindow(name.c_str(), config)),
    instance_(context.getInstance()), device_(context.getDevice()),
    vulkanSurface_(createVulkanSurface()), swapchain_(sdlWindow_, vulkanSurface_, device_)
{}

Window::Impl::~Impl() {
    if (instance_.instance) {
        instance_.instance.destroySurfaceKHR(vulkanSurface_);
        logger.logDebug("Destroyed Vulkan surface");
    } else {
        logger.logWarning("Could not destroy Vulkan surface; instance already destroyed");
    }

    SDL_DestroyWindow(sdlWindow_);
    logger.logDebug("Destroyed SDL2 window");
}

SDL_Window* Window::Impl::createSdlWindow(const char* name, const Config& config) {
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

vk::SurfaceKHR Window::Impl::createVulkanSurface() {
    VkSurfaceKHR surfaceRaw;
    if (SDL_Vulkan_CreateSurface(sdlWindow_, instance_.instance, &surfaceRaw) != SDL_TRUE) {
        throw VideoException(VideoException::Type::SurfaceCreateFailure);
    }
    logger.logDebug("Created Vulkan surface for window");

    return vk::SurfaceKHR(surfaceRaw);
}
