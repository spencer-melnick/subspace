#include "sdl_window.hpp"

// STL includes
#include <exception>

// Project includes 
#include "engine/util/logger.hpp"

using namespace std;
using namespace subspace;

SdlVulkanWindow::SdlVulkanWindow(const std::string& name, unsigned displayNum, unsigned width,
    unsigned height, uint32_t flags) :
    name_(name), displayNum_(displayNum), width_(width), height_(height)
{
    handle_ = SDL_CreateWindow(name.c_str(),
        SDL_WINDOWPOS_UNDEFINED_DISPLAY(displayNum), SDL_WINDOWPOS_UNDEFINED_DISPLAY(displayNum),
        width, height, flags);

    if (handle_ == nullptr) {
        logger.logError("SDL error: {}", SDL_GetError());
        throw std::runtime_error("Unable to create SDL window");
    }

    logger.logVerbose("Created SDL window - {}", name_);
}

SdlVulkanWindow::~SdlVulkanWindow() {
    SDL_DestroyWindow(handle_);
    logger.logVerbose("Destroyed SDL window - {}", name_);
}

SdlVulkanWindow::operator SDL_Window*() const {
    return handle_;
}

vector<const char*> SdlVulkanWindow::listRequiredExtensions() const {
    unsigned numExtensions;
    vector<const char*> extensions;

    if (SDL_Vulkan_GetInstanceExtensions(handle_, &numExtensions, nullptr) != SDL_TRUE) {
        logger.logError("Cannot get number of required Vulkan extensions - {}", SDL_GetError());
        throw std::runtime_error("Failed to get required instance extensions");
    }

    extensions.resize(numExtensions);

    if (SDL_Vulkan_GetInstanceExtensions(handle_, &numExtensions, extensions.data()) != SDL_TRUE) {
        logger.logError("Cannot get number of required Vulkan extensions - {}", SDL_GetError());
        throw std::runtime_error("Failed to get required instance extensions");
    }

    for (auto& i : extensions) {
        logger.logVerbose("Required extension: {}", i);
    }

    return extensions;
}

vk::SurfaceKHR SdlVulkanWindow::createSurface(const vk::Instance& instance) const {
    VkSurfaceKHR surface;

    if (SDL_Vulkan_CreateSurface(handle_, instance, &surface) != SDL_TRUE) {
        logger.logError("SDL Error: {}", SDL_GetError());
        throw std::runtime_error("Failed to create window surface");
    }

    return vk::SurfaceKHR(surface);
}

const std::string& SdlVulkanWindow::getName() const {
    return name_;
}

unsigned SdlVulkanWindow::getWidth() const {
    return width_;
}

unsigned SdlVulkanWindow::getHeight() const {
    return height_;
}
