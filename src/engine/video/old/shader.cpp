#include "shader.hpp"

#include "wrappers/vulkan_shader_module.hpp"

using namespace std;
using namespace subspace;

Shader::Shader(RenderContext& context, const string& vertFilename, const string& fragFilename) :
    vertShader_(new VulkanShaderModule(context.getLogicalDevice(), vertFilename)),
    fragShader_(new VulkanShaderModule(context.getLogicalDevice(), fragFilename))
{}

Shader::~Shader() = default;

const VulkanShaderModule& Shader::getVertShader() const {
    return *vertShader_;
}

const VulkanShaderModule& Shader::getFragShader() const {
    return *fragShader_;
}
