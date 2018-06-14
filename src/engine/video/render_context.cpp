#include "render_context.hpp"

#include <map>

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL_vulkan.h>

#include "../util/logger.hpp"
#include "video_exception.hpp"

using namespace std;
using namespace subspace;

class RenderContext::Impl_ {
    public:
        Impl_() {
            logger.logDebug("Creating dummy window for extension query");

            SDL_Window* dummyWindow = SDL_CreateWindow(
                "subspace",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                1, 1,
                SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN
            );
            if (dummyWindow == nullptr) {
                throw VideoException(VideoException::Type::WindowCreateFailure);
            }

            vulkanInstance_ = createVulkanInstance(dummyWindow);

            logger.logDebug("Creating dummy surface for extension query");
            VkSurfaceKHR surfaceRaw;
            if (SDL_Vulkan_CreateSurface(dummyWindow, vulkanInstance_, &surfaceRaw) != SDL_TRUE) {
                throw VideoException(VideoException::Type::SurfaceCreateFailure);
            }
            vk::SurfaceKHR dummySurface(surfaceRaw);

            auto devices = getSupportedDevices(dummySurface);
            auto& physicalDevice = (--devices.end())->second;

            vulkanInstance_.destroySurfaceKHR(dummySurface);
            SDL_DestroyWindow(dummyWindow);
            logger.logDebug("Freeing dummy window and surface");

            logger.logInfo("Using device - {}", physicalDevice.name);
            device_ = createLogicalDevice(physicalDevice);
        }


        ~Impl_() {
            device_.destroy();
            logger.logDebug("Destroyed Vulkan logical device");

            vulkanInstance_.destroy();
            logger.logDebug("Destroyed Vulkan instance");
        }


    private:
        struct DeviceWrapper_ {
            vk::PhysicalDevice vulkanDevice;
            unsigned usedQueueFamily;
            std::string name;
        };

        using DeviceList_ = std::multimap<unsigned, DeviceWrapper_>;

        vk::Instance vulkanInstance_;
        vk::Device device_;


        DeviceList_ getSupportedDevices(vk::SurfaceKHR& surface) {
            DeviceList_ result;
            auto devices = vulkanInstance_.enumeratePhysicalDevices();

            if (devices.empty()) {
                throw VideoException(VideoException::Type::NoVulkanDevices);
            }
            logger.logVerbose("Found {} supported Vulkan devices:", devices.size());

            for (auto& i : devices) {
                DeviceWrapper_ device;
                device.vulkanDevice = i;

                auto deviceProperties = device.vulkanDevice.getProperties();
                device.name = string(deviceProperties.deviceName);

                if (findQueueFamily(device, surface)) {
                    result.insert({rateDeviceSuitability(device), device});
                }
            }

            if (result.empty()) {
                throw VideoException(VideoException::Type::NoSupportedDevices);
            }

            return result;
        }


        static vk::Instance createVulkanInstance(SDL_Window* window) {
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

        static std::vector<const char*> getRequiredExtensions(SDL_Window* window) {
            vector<const char*> result;
            unsigned numExtensions;
            const char** extensionList;

            if (!SDL_Vulkan_GetInstanceExtensions(window, &numExtensions, nullptr)) {
                throw VideoException(VideoException::Type::GetSurfaceExtensionsFailure);
            }

            extensionList = new const char* [numExtensions];

            if (!SDL_Vulkan_GetInstanceExtensions(window, &numExtensions, extensionList)) {
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

        static int rateDeviceSuitability(DeviceWrapper_& physicalDevice) {
            int score = 0;
            auto properties = physicalDevice.vulkanDevice.getProperties();

            switch (properties.deviceType) {
                case vk::PhysicalDeviceType::eDiscreteGpu:
                    score += 1000000;
                    logger.logVerbose("Discrete GPU - {}", physicalDevice.name);
                    break;
                case vk::PhysicalDeviceType::eIntegratedGpu:
                    score += 50000;
                    logger.logVerbose("Integrated GPU - {}", physicalDevice.name);
                    break;
                default:
                    logger.logVerbose("CPU or virtual device - {}", physicalDevice.name);
                    break;
            }

            score += properties.limits.maxImageDimension2D;
            logger.logVerbose("Device score: {}", score);

            return score;
        }

        static vk::Device createLogicalDevice(DeviceWrapper_& physicalDevice) {
            float queuePriority = 1.0f;
            
            vk::DeviceQueueCreateInfo queueInfo(
                {},
                physicalDevice.usedQueueFamily,
                1,
                &queuePriority
            );
            vk::DeviceCreateInfo deviceInfo(
                {},
                1,
                &queueInfo
            );

            return physicalDevice.vulkanDevice.createDevice(deviceInfo);
        }

        static bool findQueueFamily(DeviceWrapper_& device, vk::SurfaceKHR& surface) {
            const auto requriredFlags = vk::QueueFlagBits::eGraphics;
            auto queueProperties = device.vulkanDevice.getQueueFamilyProperties();

            for (unsigned i = 0; i < queueProperties.size(); i++) {
                auto family = queueProperties[i];

                bool flagSupport = (family.queueFlags & requriredFlags) == requriredFlags;
                bool presentSupport = device.vulkanDevice.getSurfaceSupportKHR(i, surface);

                if (family.queueCount > 0 && flagSupport && presentSupport) {
                    device.usedQueueFamily = i;
                    return true;
                }
            }
            
            logger.logWarning("{} is not compatible", device.name);
            return false;
        }
};

RenderContext::RenderContext() :
    impl_(new Impl_)
{}

RenderContext::~RenderContext() = default;
