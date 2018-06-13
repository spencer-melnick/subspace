#include "video_exception.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include "../util/logger.hpp"

namespace subspace {
    void initializeVideo() {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
            logger.logError("SDL2 Error: {}", SDL_GetError());
            throw VideoException(VideoException::Type::Sdl2InitFailure);
        }
        logger.logDebug("Initialized SDL2 video subsystem");

        if (SDL_Vulkan_LoadLibrary(nullptr) != 0) {
            throw VideoException(VideoException::Type::VulkanInitFailure);
        }
        logger.logDebug("Loaded Vulkan libraries");
    }

    void terminateVideo() {
        SDL_Vulkan_UnloadLibrary();
        logger.logDebug("Unloaded Vulkan libraries");

        SDL_Quit();
        logger.logDebug("Terminated SDL2");
    }
}
