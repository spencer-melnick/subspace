#include "video_exception.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

namespace subspace {
    void initializeVideo() {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
            throw VideoException(VideoException::Type::Sdl2InitFailure);
        }

        if (SDL_Vulkan_LoadLibrary(nullptr) != 0) {
            throw VideoException(VideoException::Type::VulkanInitFailure);
        }
    }

    void terminateVideo() {
        SDL_Vulkan_UnloadLibrary();

        SDL_Quit();
    }
}
