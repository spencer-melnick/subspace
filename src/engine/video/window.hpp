#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Class that provides simple window functionality by wrapping the SDL2 window
 * interface.
 */

#include <string>
#include <memory>

#include "video_exception.hpp"
#include "render_context.hpp"
#include "../util/config.hpp"

namespace subspace {
    class Window {
        /**
         * Class that provides simple window functionality by wrapping the SDL2 window
         * interface.
         */

        public:
            Window(RenderContext& context, const std::string& name, const Config& config);
            ~Window();

        private:
            class Impl_;
            std::unique_ptr<Impl_> impl_;
    };
}
