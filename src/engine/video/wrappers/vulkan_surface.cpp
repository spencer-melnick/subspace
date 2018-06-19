#include "vulkan_surface.hpp"

#include "../../util/logger.hpp"
#include "../video_exception.hpp"
#include "vulkan_instance.hpp"
#include "sdl_window.hpp"

using namespace subspace;

VulkanSurface::VulkanSurface(const VulkanInstance& instance, const SdlWindow& window) :
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
    instance_->destroySurfaceKHR(handle_);
}

VulkanSurface::operator VkSurfaceKHR() const {
    return static_cast<VkSurfaceKHR>(handle_);
}

VulkanSurface::operator const vk::SurfaceKHR&() const {
    return handle_;
}
