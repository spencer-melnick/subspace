#include "window.hpp"

#include "window_impl.hpp"

Window::Window(RenderContext& context, const std::string& name, const Config& config) :
    impl_(new Impl_(*context.impl_, name, config))
{}

Window::~Window() = default;
