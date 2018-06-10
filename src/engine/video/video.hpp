#pragma once

/**
 * @file
 * @author Spencer Melnick <smelnick97@gmail.com>
 * 
 * Main include file for the video systems. Used for window and Vulkan device
 * creation.
 */

#include "video_exception.hpp"
#include "window.hpp"
#include "render_context.hpp"

namespace subspace {
    /**
     * Initializes the video subsystem and all necessary components, including
     * the SDL2 video subsystem. Throws a VideoException on failure.
     */
    void initializeVideo();

    /**
     * Terminates the video subsystem and all components started by
     * initializeVideo()
     */
    void terminateVideo();
}
