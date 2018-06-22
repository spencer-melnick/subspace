#pragma once

#include <memory>

#include "render_context.hpp"
#include "window.hpp"
#include "shader.hpp"

namespace subspace {
    class VulkanPipeline;

    class Renderer {
        public:
            Renderer(const Window& window, const Shader& shader);
            ~Renderer();

        private:
            const RenderContext& context_;
            const Window& window_;
            const Shader& shader_;

            std::unique_ptr<VulkanPipeline> pipeline_;
    };
}
