#include "vulkan_shader_module.hpp"

#include "../../util/logger.hpp"
#include "../../util/simple_file.hpp"
#include "vulkan_logical_device.hpp"

using namespace std;
using namespace subspace;

VulkanShaderModule::VulkanShaderModule(const VulkanLogicalDevice& device, const string& filename) :
    device_(device)
{
    string code = simpleFileRead(filename);
    vk::ShaderModuleCreateInfo createInfo({}, code.length(),
        reinterpret_cast<uint32_t*>(&code[0]));

    handle_ = device_->createShaderModule(createInfo);
    logger.logDebug("Created shader module for {}", filename);
}

VulkanShaderModule::~VulkanShaderModule() {
    device_->destroyShaderModule(handle_);
    logger.logDebug("Destroyed shader module");
}

VulkanShaderModule::operator const vk::ShaderModule&() const {
    return handle_;
}
