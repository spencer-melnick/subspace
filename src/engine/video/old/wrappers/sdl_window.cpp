#include "sdl_window.hpp"

#include "../../util/logger.hpp"
#include "../video_exception.hpp"

using namespace std;
using namespace subspace;

SdlWindow::SdlWindow(const std::string& name, unsigned displayNum, unsigned width, unsigned height, uint32_t flags) {
    handle_ = SDL_CreateWindow(name.c_str(),
        SDL_WINDOWPOS_UNDEFINED_DISPLAY(displayNum), SDL_WINDOWPOS_UNDEFINED_DISPLAY(displayNum),
        width, height, flags);

    if (handle_ == nullptr) {
        throw VideoException(VideoException::Type::WindowCreateFailure);
    }
}

SdlWindow::~SdlWindow() {
    SDL_DestroyWindow(handle_);
}

SdlWindow::operator SDL_Window*() const {
    return handle_;
}

vector<const char*> SdlWindow::listRequiredExtensions() const {
    unsigned numExtensions;
    vector<const char*> extensions;

    if (SDL_Vulkan_GetInstanceExtensions(handle_, &numExtensions, nullptr) != SDL_TRUE) {
        logger.logError("Cannot get number of required Vulkan extensions - {}", SDL_GetError());
        throw VideoException(VideoException::Type::GetSurfaceExtensionsFailure);
    }

    extensions.resize(numExtensions);

    if (SDL_Vulkan_GetInstanceExtensions(handle_, &numExtensions, extensions.data()) != SDL_TRUE) {
        logger.logError("Cannot get number of required Vulkan extensions - {}", SDL_GetError());
        throw VideoException(VideoException::Type::GetSurfaceExtensionsFailure);
    }

    for (auto& i : extensions) {
        logger.logVerbose("Required extension: {}", i);
    }

    return extensions;
}
