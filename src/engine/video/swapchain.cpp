#include "swapchain.hpp"
#include "../util/logger.hpp"

//IM SORRY THE NAMING CONVENTIONS ARE WEIRD STEPHEN WROTE ThIS CODE
using namespace subspace;

SwapChain::SwapChain(vk::SurfaceKHR& vulkanSurface, RenderContext::Impl_& context)
	:context_(context)
{
	format_ = chooseSurfaceFormat();
	mode_ = choosePresentMode();
	auto extent = chooseSwapExtent();
	uint32_t numImages = chooseImageCount();

	vk::SwapchainCreateInfoKHR createInfo(
	{}, vulkanSurface_, numImages, format_.format,
		format_.colorSpace, extent, 1, vk::ImageUsageFlagBits::eColorAttachment,
		vk::SharingMode::eExclusive, 0, nullptr, vk::SurfaceTransformFlagBitsKHR::eIdentity,
		vk::CompositeAlphaFlagBitsKHR::eOpaque, mode_, true);
	logger.logDebug("Created window surface swapchain");

	swapchain_ = context_.device_.createSwapchainKHR(createInfo);
	images_ = context_.device_.getSwapchainImagesKHR(swapchain_);
	images_.resize(numImages);
	imageViews_ = createImageViews(n);
}

vk::PresentModeKHR SwapChain::choosePresentMode()
{
	auto& physicalDevice = context_.physicalDevice_.vulkanDevice;
	auto availableModes = physicalDevice.getSurfacePresentModesKHR(vulkanSurface_);

	// Try for mailbox present mode - triple buffering!
	for (const auto& i : availableModes) {
		if (i == vk::PresentModeKHR::eMailbox) {
			return i;
		}
	}

	// Immediate mode has best support
	for (const auto& i : availableModes) {
		if (i == vk::PresentModeKHR::eImmediate) {
			return i;
		}
	}

	// FIFO is guaranteed, but not always well supported
	return vk::PresentModeKHR::eFifo;
}

uint32_t SwapChain::chooseImageCount() {
	auto& device = context_.physicalDevice_.vulkanDevice;
	vk::SurfaceCapabilitiesKHR capabilities = device.getSurfaceCapabilitiesKHR(vulkanSurface_);

	uint32_t result = capabilities.minImageCount + 1;

	// MaxImageCount of 0 means that there is no limit
	// If we exceed the limit, go with the limit
	if (result > capabilities.maxImageCount && capabilities.maxImageCount > 0) {
		return capabilities.maxImageCount;
	}

	return result;
}

std::vector<vk::ImageView> SwapChain::createImageViews() {
	std::vector<vk::ImageView> imageViews;

	for (auto& i : images_) {
		vk::ImageViewCreateInfo createInfo({}, i, vk::ImageViewType::e2D, format_.format,
			vk::ComponentMapping(),
			vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
		imageViews.push_back(context_.device_.createImageView(createInfo));
	}
	logger.logDebug("Created swapchain image views");

	return imageViews;
}

vk::Extent2D SwapChain::chooseSwapExtent() {
	auto& device = context_.physicalDevice_.vulkanDevice;
	vk::SurfaceCapabilitiesKHR capabilities = device.getSurfaceCapabilitiesKHR(vulkanSurface_);

	// Special case if extent is uint32 max
	if (capabilities.currentExtent.width == numeric_limits<uint32_t>::max()) {
		int width, height;
		SDL_Vulkan_GetDrawableSize(sdlWindow_, &width, &height);

		vk::Extent2D result{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

		result.width = max(result.width, capabilities.minImageExtent.width);
		result.width = min(result.width, capabilities.maxImageExtent.width);

		result.height = max(result.height, capabilities.minImageExtent.height);
		result.height = min(result.height, capabilities.maxImageExtent.height);

		return result;
	}
	else {
		return capabilities.currentExtent;
	}
}

VkSwapchainKHR getSwapChain() {
	return swapchain_;
}