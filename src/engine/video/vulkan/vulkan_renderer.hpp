#pragma once

// STL includes
#include <string>
#include <vector>

// Library includes
#include <vulkan/vulkan.hpp>

// Project includes 
#include "../renderer.hpp"
#include "context.hpp"

namespace subspace {
    class VulkanRenderer: public IRenderer {
        public:
            VulkanRenderer();

            // Interface functions
            virtual void draw() override;

            // Getters
            const Context& getContext() const;
            const vk::RenderPass& getRenderPass() const;
            const vk::Pipeline& getPipeline() const;

        private:
            // Initialization helpers
            void createRenderPass();
            void createPipeline();

            // Member variables
            Context context_;
            vk::UniqueRenderPass renderPass_;
            vk::UniquePipelineLayout pipelineLayout_;
            vk::UniquePipeline pipeline_;
    };
}
