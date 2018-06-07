#include "video_exception.hpp"

#include <SDL2/SDL.h>

namespace subspace {
    void initializeVideo() {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
            throw VideoException(VideoException::Type::SDL2_INIT_FAILURE);
        }
    }
}
