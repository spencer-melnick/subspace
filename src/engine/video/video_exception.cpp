#include "video_exception.hpp"

using namespace subspace;

VideoException::VideoException(const VideoException::Type& type) :
    type_(type)
{};

const char* VideoException::what() const noexcept {
    switch (type_) {
        case Type::SDL2_INIT_FAILURE:
            return "Failed to initialize SDL2 system";
        case Type::WINDOW_CREATE_FAILURE:
            return "Failed to create window";
        default:
            return "No error";
    }
}

VideoException::Type VideoException::getType() const noexcept {
    return type_;
}
