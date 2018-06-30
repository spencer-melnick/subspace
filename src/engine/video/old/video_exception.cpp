#include "video_exception.hpp"

using namespace subspace;

VideoException::VideoException(const VideoException::Type& type) :
    type_(type)
{}

const char* VideoException::what() const noexcept {
    switch (type_) {
        case Type::Sdl2InitFailure:
            return "Failed to initialize SDL2 system";
        case Type::VulkanInitFailure:
            return "Failed to load Vulkan library";
        case Type::GetSurfaceExtensionsFailure:
            return "Failed to get required Vulkan surface extensions";
        case Type::NoVulkanDevices:
            return "No Vulkan devices available";
        case Type::NoSupportedDevices:
            return "No Vulkan devices with required features available";
        case Type::InstanceCreateFailure:
            return "Failed to create a Vulkan instance";
        case Type::SurfaceCreateFailure:
            return "Failed to create a Vulkan surface";
        case Type::WindowCreateFailure:
           return "Failed to create window";
    }

    return "";
}

VideoException::Type VideoException::getType() const noexcept {
    return type_;
}
