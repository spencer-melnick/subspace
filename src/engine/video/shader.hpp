#pragma once

#include <string>
#include <memory>

#include "render_context.hpp"

namespace subspace {
    class VulkanShaderModule;

    class Shader {
        public:
            Shader(RenderContext& context, const std::string& vertFilename,
                const std::string& fragFilename);
            ~Shader();

        private:
            std::unique_ptr<VulkanShaderModule> vertShader_, fragShader_;
    };
}
