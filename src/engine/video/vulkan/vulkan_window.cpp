#include "vulkan_window.hpp"

// Project includes
#include "engine/util/logger.hpp"

using namespace subspace;

VulkanWindow::VulkanWindow(const VulkanRenderer& renderer, const std::string& name,
    const Config& config) :
        window_(name, 0, config.getXResolution(), config.getYResolution(), SDL_WINDOW_VULKAN),
        surface_(window_.createSurface(renderer.getContext().getInstance()), {
            renderer.getContext().getInstance()
        }),
        swapchain_(renderer.getContext(), window_, *surface_)
{
    logger.logVerbose("Created window");
}

void VulkanWindow::swap() {

}
