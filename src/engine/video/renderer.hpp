#pragma once

#include <vector>
#include <memory>

#include "render_context.hpp"
#include "window.hpp"
#include "shader.hpp"

namespace subspace {
    class VulkanPipeline;
    class VulkanFramebuffers;
    class VulkanPrimaryCommandBuffers;

    class Renderer {
        public:
            Renderer(const Window& window, const Shader& shader);
            ~Renderer();

        private:
            static const unsigned maxSynchronousFrames_;

            const RenderContext& context_;
            const Window& window_;
            const Shader& shader_;

            unsigned currentFrame_ = 0;

            std::unique_ptr<VulkanPipeline> pipeline_;
            std::unique_ptr<VulkanFramebuffers> framebuffers_;
            std::unique_ptr<VulkanPrimaryCommandBuffers> primaryCommands_;
    };
}
