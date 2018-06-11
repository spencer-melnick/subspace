#include "render_context.hpp"

#include <SDL2/SDL_vulkan.h>
#include <fmt/format.h>

#include "../util/logger.hpp"

using namespace std;

namespace subspace {
    RenderContext::RenderContext(const Window& window) {
        vulkanInstance_ = createVulkanInstance(window);
    }

    RenderContext::~RenderContext() {
        vulkanInstance_.destroy();
        logger.logDebug("Destroyed Vulkan instance");
    }

    vk::Instance RenderContext::createVulkanInstance(const Window& window) {
        vk::ApplicationInfo appInfo(
            "Subspace",
            1,
            nullptr,
            0,
            VK_MAKE_VERSION(1, 0, 0)
        );

        vector<const char*> requiredExtensions = getRequiredExtensions(window);

        vk::InstanceCreateInfo instanceInfo(
            {},
            &appInfo,
            0,
            nullptr,
            requiredExtensions.size(),
            requiredExtensions.data()
        );

        vk::Instance instance;

        if (vk::createInstance(&instanceInfo, nullptr, &instance) != vk::Result::eSuccess) {
            throw VideoException(VideoException::Type::InstanceCreateFailure);
        }

        logger.logDebug("Created Vulkan instance");

        return instance;
    }

    vector<const char*> RenderContext::getRequiredExtensions(const Window& window) {
        unsigned int numExtensions;
        vector<const char*> result;
        const char** extensionList;

        if (!SDL_Vulkan_GetInstanceExtensions(window.sdlWindow_, &numExtensions, nullptr)) {
            throw VideoException(VideoException::Type::GetSurfaceExtensionsFailure);
        }

        extensionList = new const char* [numExtensions];

        if (!SDL_Vulkan_GetInstanceExtensions(window.sdlWindow_, &numExtensions, extensionList)) {
            throw VideoException(VideoException::Type::GetSurfaceExtensionsFailure);
        }

        logger.logVerbose("Required Vulkan extensions for window rendering:");
        for (unsigned int i = 0; i < numExtensions; i++) {
            result.push_back(extensionList[i]);
            logger.logVerbose(extensionList[i]);
        }

        delete[] extensionList;
        return result;
    }
}