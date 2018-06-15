#include "window_impl.hpp"

#include "../util/logger.hpp"

using namespace std;
using namespace subspace;

Window::Impl_::Impl_(RenderContext::Impl_& context, const std::string& name, const Config& config) :
    context_(context)
{
    sdlWindow_ = createSdlWindow(name.c_str(), config);
    vulkanSurface_ = createVulkanSurface(sdlWindow_, context_.instance_);
    swapchain_ = createSwapchain();
}

Window::Impl_::~Impl_() {
    if (context_.instance_) {
        context_.device_.destroySwapchainKHR(swapchain_);
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

vk::SurfaceKHR Window::Impl_::createVulkanSurface(SDL_Window* window, vk::Instance& instance) {
    VkSurfaceKHR surfaceRaw;
    if (SDL_Vulkan_CreateSurface(window, instance, &surfaceRaw) != SDL_TRUE) {
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

vk::PresentModeKHR Window::Impl_::choosePresentMode() {
    auto& physicalDevice = context_.physicalDevice_.vulkanDevice;
    auto availableModes = physicalDevice.getSurfacePresentModesKHR(vulkanSurface_);

    // Try for mailbox present mode - triple buffering!
    for (const auto& i : availableModes) {
        if (i == vk::PresentModeKHR::eMailbox) {
            return i;
        }
    }

    // Immediate mode has best support
    for (const auto& i : availableModes) {
        if (i == vk::PresentModeKHR::eImmediate) {
            return i;
        }
    }

    // FIFO is guaranteed, but not always well supported
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Window::Impl_::chooseSwapExtent() {
    auto& device = context_.physicalDevice_.vulkanDevice;
    vk::SurfaceCapabilitiesKHR capabilities = device.getSurfaceCapabilitiesKHR(vulkanSurface_);

    // Special case if extent is uint32 max
    if (capabilities.currentExtent.width == numeric_limits<uint32_t>::max()) {
        int width, height;
        SDL_Vulkan_GetDrawableSize(sdlWindow_, &width, &height);

        vk::Extent2D result {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        result.width = max(result.width, capabilities.minImageExtent.width);
        result.width = min(result.width, capabilities.maxImageExtent.width);

        result.height = max(result.height, capabilities.minImageExtent.height);
        result.height = min(result.height, capabilities.maxImageExtent.height);

        return result;
    } else {
        return capabilities.currentExtent;
    }
}

uint32_t Window::Impl_::chooseImageCount() {
    auto& device = context_.physicalDevice_.vulkanDevice;
    vk::SurfaceCapabilitiesKHR capabilities = device.getSurfaceCapabilitiesKHR(vulkanSurface_);

    uint32_t result = capabilities.minImageCount + 1;

    // MaxImageCount of 0 means that there is no limit
    // If we exceed the limit, go with the limit
    if (result > capabilities.maxImageCount && capabilities.maxImageCount > 0) {
        return capabilities.maxImageCount;
    }

    return result;
}

vk::SwapchainKHR Window::Impl_::createSwapchain() {
    auto surfaceFormat = chooseSurfaceFormat();
    auto presentMode = choosePresentMode();
    auto extent = chooseSwapExtent();
    uint32_t numImages = chooseImageCount();

    vk::SwapchainCreateInfoKHR createInfo(
        {}, vulkanSurface_, numImages, surfaceFormat.format, 
        surfaceFormat.colorSpace, extent, 1, vk::ImageUsageFlagBits::eColorAttachment,
        vk::SharingMode::eExclusive, 0, nullptr, vk::SurfaceTransformFlagBitsKHR::eIdentity,
        vk::CompositeAlphaFlagBitsKHR::eOpaque, presentMode, true);

    vk::SwapchainKHR swapchain = context_.device_.createSwapchainKHR(createInfo);
    logger.logDebug("Created window surface swapchain");
    return swapchain;
}
