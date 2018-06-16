#include "render_context.hpp"

#include "render_context_impl.hpp"

#include <vulkan/vulkan.hpp>

using namespace subspace;

RenderContext::RenderContext() :
    impl_(new Impl)
{}

RenderContext::~RenderContext() = default;

const RenderContext::InstanceHandle& RenderContext::getInstance() const {
    return impl_->getInstance();
}

const RenderContext::DeviceHandle& RenderContext::getDevice() const {
    return impl_->getDevice();
}
