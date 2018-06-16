#include "shader.hpp"

#include "shader_impl.hpp"

using namespace std;
using namespace subspace;

Shader::Shader(RenderContext& context, const string& vertFilename, const string& fragFilename) :
    impl_(new Impl(context, vertFilename, fragFilename))
{}

Shader::~Shader() = default;
