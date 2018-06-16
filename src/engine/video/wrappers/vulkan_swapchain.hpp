#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace subspace {
	class RenderContext;
	class SdlWindow;
	class VulkanSurface;

	class VulkanSwapchain {
		public:
			VulkanSwapchain(const RenderContext& context, const SdlWindow& window,
				const VulkanSurface& vulkanSurface);
			~VulkanSwapchain();
			std::vector<vk::ImageView> imageViews;
			VkSwapchainKHR swapchainKHR;

		private:
			const RenderContext& context_;
			const VulkanSurface& vulkanSurface_;
			vk::SurfaceFormatKHR format_;
			vk::PresentModeKHR mode_;
			std::vector<vk::Image> images_;

			vk::SurfaceFormatKHR chooseSurfaceFormat();
			vk::PresentModeKHR choosePresentMode();
			vk::Extent2D chooseSwapExtent(const SdlWindow& window);
			uint32_t chooseImageCount();
			std::vector<vk::ImageView> createImageViews();
	};
}


