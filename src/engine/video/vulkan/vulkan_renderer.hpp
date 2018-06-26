#pragma once

// STL includes
#include <string>
#include <vector>

// Library includes
#include <vulkan/vulkan.hpp>

// Project includes 
#include "context.hpp"

namespace subspace {
    class VulkanRenderer {
        public:
            VulkanRenderer() = default;

        private:
            Context context_;
    };
}
