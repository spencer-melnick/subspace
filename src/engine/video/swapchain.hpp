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
		SwapChain(SDL_Window* window, vk::SurfaceKHR& vulkanSurface, const RenderContext::DeviceHandle& device);
		~SwapChain();
		std::vector<vk::ImageView> imageViews;
		VkSwapchainKHR swapchainKHR;

	private:
		vk::SurfaceKHR vulkanSurface_;
		const RenderContext::DeviceHandle& device_;
		vk::SurfaceFormatKHR format_;
		vk::PresentModeKHR mode_;
		std::vector<vk::Image> images_;

		vk::SurfaceFormatKHR chooseSurfaceFormat();
		vk::PresentModeKHR choosePresentMode();
		vk::Extent2D chooseSwapExtent(SDL_Window* window);
		uint32_t chooseImageCount();
		std::vector<vk::ImageView> createImageViews();
	};
}


