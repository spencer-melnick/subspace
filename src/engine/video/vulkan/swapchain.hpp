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
			Swapchain(const Context& context, const SdlVulkanWindow& window,
				const vk::SurfaceKHR& vulkanSurface, const vk::RenderPass& renderPass,
				vk::SwapchainKHR oldSwapchain = vk::SwapchainKHR(nullptr));
			~Swapchain();

			// Move assignment
			Swapchain& operator=(Swapchain&& other);

			// Cast to wrapped type
			operator const vk::SwapchainKHR&() const;

			// Getters
			const vk::Extent2D& getExtent() const;
			const std::vector<vk::UniqueImageView>& getImageViews() const;
			const vk::Framebuffer& getFramebuffer(unsigned index) const;

		private:
			// Initialization helpers
			void choosePresentMode();
			void chooseSwapExtent(const SdlVulkanWindow& window);
			uint32_t chooseImageCount();
			void createImageViews();
			void createFramebuffers(const vk::RenderPass& renderPass);

			// Member variables
			vk::UniqueSwapchainKHR handle_;
			vk::PresentModeKHR mode_;
			vk::Extent2D extent_;
			std::vector<vk::Image> images_;
			std::vector<vk::UniqueImageView> imageViews_;
			std::vector<vk::UniqueFramebuffer> framebuffers_;

			// References to object dependencies
			const Context& context_;
			const vk::SurfaceKHR& vulkanSurface_;
	};
}


