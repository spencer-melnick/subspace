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

			operator const vk::SwapchainKHR&() const;
			const std::vector<vk::ImageView>& getImageViews() const;

		private:
			void chooseSurfaceFormat();
			void choosePresentMode();
			vk::Extent2D chooseSwapExtent(const SdlWindow& window);
			uint32_t chooseImageCount();
			void createImageViews();

			vk::SwapchainKHR handle_;
			vk::SurfaceFormatKHR format_;
			vk::PresentModeKHR mode_;
			std::vector<vk::Image> images_;
			std::vector<vk::ImageView> imageViews_;

			const RenderContext& context_;
			const VulkanSurface& vulkanSurface_;
	};
}


