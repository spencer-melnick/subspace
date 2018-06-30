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
			const vk::SurfaceFormatKHR& getFormat() const;
			const vk::Extent2D& getExtent() const;
			const std::vector<vk::ImageView>& getImageViews() const;

		private:
			void chooseSurfaceFormat();
			void choosePresentMode();
			void chooseSwapExtent(const SdlWindow& window);
			uint32_t chooseImageCount();
			void createImageViews();

			vk::SwapchainKHR handle_;
			vk::SurfaceFormatKHR format_;
			vk::PresentModeKHR mode_;
			vk::Extent2D extent_;
			std::vector<vk::Image> images_;
			std::vector<vk::ImageView> imageViews_;

			const RenderContext& context_;
			const VulkanSurface& vulkanSurface_;
	};
}


