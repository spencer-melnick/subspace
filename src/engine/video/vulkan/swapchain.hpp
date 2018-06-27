#pragma once

// STL includes
#include <vector>

// Library includes
#include <vulkan/vulkan.hpp>

// Project includes
#include "context.hpp"
#include "sdl_window.hpp"

namespace subspace {

	class Swapchain {
		public:
			Swapchain(const Context& context, const SdlWindow& window,
				const vk::SurfaceKHR& vulkanSurface);

			// Cast to wrapped type
			operator const vk::SwapchainKHR&() const;

			// Getters
			const vk::SurfaceFormatKHR& getFormat() const;
			const vk::Extent2D& getExtent() const;
			const std::vector<vk::UniqueImageView>& getImageViews() const;

		private:
			// Initialization helpers
			void chooseSurfaceFormat();
			void choosePresentMode();
			void chooseSwapExtent(const SdlWindow& window);
			uint32_t chooseImageCount();
			void createImageViews();
			void createRenderPass();
			void createFramebuffers();

			// Member variables
			vk::UniqueSwapchainKHR handle_;
			vk::SurfaceFormatKHR format_;
			vk::PresentModeKHR mode_;
			vk::Extent2D extent_;
			std::vector<vk::Image> images_;
			std::vector<vk::UniqueImageView> imageViews_;
			vk::UniqueRenderPass renderPass_;
			std::vector<vk::UniqueFramebuffer> framebuffers_;

			// References to object dependencies
			const Context& context_;
			const vk::SurfaceKHR& vulkanSurface_;
	};
}


