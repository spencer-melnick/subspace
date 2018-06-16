#pragma once

#include <string>
#include <memory>

#include "render_context.hpp"

namespace subspace {
    class Shader {
        public:
            Shader(RenderContext& context, const std::string& vertFilename,
                const std::string& fragFilename);
            ~Shader();

        private:
            class Impl;

            std::unique_ptr<Impl> impl_;
    }
}
