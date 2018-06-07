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

namespace subspace {
    void initializeVideo();
}
