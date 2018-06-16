#include "vulkan_surface.hpp"

#include "../../util/logger.hpp"
#include "../video_exception.hpp"

using namespace subspace;

VulkanSurface::VulkanSurface(const vk::Instance& instance, SDL_Window* window) :
    instance_(instance)
{
    VkSurfaceKHR rawSurface;

    if (SDL_Vulkan_CreateSurface(window, instance_, &rawSurface) != SDL_TRUE) {
        logger.logError("Failed to create Vulkan surface: {}", SDL_GetError());
        throw VideoException(VideoException::Type::SurfaceCreateFailure);
    }

    handle_ = rawSurface;
}

VulkanSurface::~VulkanSurface() {
    instance_.destroySurfaceKHR(handle_);
}

const vk::SurfaceKHR& VulkanSurface::getHandle() const {
    return handle_;
}
