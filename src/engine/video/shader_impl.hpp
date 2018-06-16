#include "shader.hpp"

namespace subspace {
    class Shader::Impl {
        public:
            class Module {

            };

            Impl(RenderContext& context, const std::string& vertFilename,
                const std::string& fragFilename);
            ~Impl();

        private:

    }
}
