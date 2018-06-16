#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include "render_context_impl.hpp"

namespace subspace {
	class SwapChain
	{
	public:
		SwapChain(vk::SurfaceKHR& vulkanSurface, RenderContext::Impl_& context);
		vk::SurfaceFormatKHR chooseSurfaceFormat();
		vk::PresentModeKHR choosePresentMode();
		vk::Extent2D chooseSwapExtent();
		uint32_t chooseImageCount();
		std::vector<vk::ImageView> createImageViews();

	private:

		vk::SurfaceKHR vulkanSurface_;
		RenderContext::Impl_& context_;
		vk::SurfaceFormatKHR format_;
		vk::PresentModeKHR mode_;
		std::vector<vk::Image> images_;
		std::vector<vk::ImageView> imageViews_;
		VkSwapchainKHR swapchain_;
	};
}


