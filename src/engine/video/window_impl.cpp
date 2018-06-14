#include "window_impl.hpp"

#include "../util/logger.hpp"

using namespace std;
using namespace subspace;

Window::Impl_::Impl_(RenderContext::Impl_& context, const std::string& name, const Config& config) :
    vulkanInstance_(context.vulkanInstance_), vulkanDevice_(context.device_)
{
    Uint32 flags = SDL_WINDOW_VULKAN;

    if (config.isFullscreen()) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    sdlWindow_ = SDL_CreateWindow(
        name.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        config.getXResolution(), config.getYResolution(),
        flags
    );

    if (sdlWindow_ == nullptr) {
        logger.logError("SDL2 error: {}", SDL_GetError());
        throw VideoException(VideoException::Type::WindowCreateFailure);
    }
    logger.logDebug("Created SDL2 window {}", name);

    VkSurfaceKHR surfaceRaw;
    if (SDL_Vulkan_CreateSurface(sdlWindow_, vulkanInstance_, &surfaceRaw) != SDL_TRUE) {
        throw VideoException(VideoException::Type::SurfaceCreateFailure);
    }
    logger.logDebug("Created Vulkan surface for window {}", name);

    vulkanSurface_ = surfaceRaw;
}

Window::Impl_::~Impl_() {
    if (vulkanInstance_) {
        vulkanInstance_.destroySurfaceKHR(vulkanSurface_);
        logger.logDebug("Destroyed Vulkan surface");
    } else {
        logger.logWarning("Could not destroy Vulkan surface; instance already destroyed");
    }

    SDL_DestroyWindow(sdlWindow_);
    logger.logDebug("Destroyed SDL2 window");
}
