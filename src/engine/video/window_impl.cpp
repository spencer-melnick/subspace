#include "window_impl.hpp"

#include "../util/logger.hpp"

using namespace std;
using namespace subspace;

Window::Impl_::Impl_(RenderContext::Impl_& context, const std::string& name, const Config& config) :
    context_(context)
{
    sdlWindow_ = createSdlWindow(name.c_str(), config);
    vulkanSurface_ = createVulkanSurface(sdlWindow_, context_.instance_);
}

Window::Impl_::~Impl_() {
    if (context_.instance_) {
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

vk::SurfaceKHR Window::Impl_::createVulkanSurface(SDL_Window* window, vk::Instance& instance) {
    VkSurfaceKHR surfaceRaw;
    if (SDL_Vulkan_CreateSurface(window, instance, &surfaceRaw) != SDL_TRUE) {
        throw VideoException(VideoException::Type::SurfaceCreateFailure);
    }
    logger.logDebug("Created Vulkan surface for window");

    return vk::SurfaceKHR(surfaceRaw);
}
