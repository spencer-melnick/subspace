#include "render_context.hpp"

#include "render_context_impl.hpp"

using namespace subspace;

RenderContext::RenderContext() :
    impl_(new Impl_)
{}

RenderContext::~RenderContext() = default;
