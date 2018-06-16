#include "vulkan_swapchain.hpp"

#include <limits>

#include <SDL2/SDL_vulkan.h>

#include "../../util/logger.hpp"
#include "../render_context.hpp"
#include "sdl_window.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_physical_device.hpp"
#include "vulkan_logical_device.hpp"
#include "vulkan_surface.hpp"

using namespace subspace;

VulkanSwapchain::VulkanSwapchain(const RenderContext& context, const SdlWindow& window, const VulkanSurface& vulkanSurface) :
	context_(context), vulkanSurface_(vulkanSurface)
{
	format_ = chooseSurfaceFormat();
	mode_ = choosePresentMode();
	auto extent = chooseSwapExtent(window);
	uint32_t numImages = chooseImageCount();

	vk::SwapchainCreateInfoKHR createInfo(
	{}, vulkanSurface_.getHandle(), numImages, format_.format,
		format_.colorSpace, extent, 1, vk::ImageUsageFlagBits::eColorAttachment,
		vk::SharingMode::eExclusive, 0, nullptr, vk::SurfaceTransformFlagBitsKHR::eIdentity,
		vk::CompositeAlphaFlagBitsKHR::eOpaque, mode_, true);
	logger.logDebug("Created window surface swapchain");

	swapchainKHR = context_.getLogicalDevice().getHandle().createSwapchainKHR(createInfo);
	images_ = context_.getLogicalDevice().getHandle().getSwapchainImagesKHR(swapchainKHR);
	images_.resize(numImages);
	imageViews = createImageViews();
}

VulkanSwapchain::~VulkanSwapchain() {
	for (auto& i : imageViews) {
		context_.getLogicalDevice().getHandle().destroyImageView(i);
	}
	logger.logDebug("Destroyed image views");
	context_.getLogicalDevice().getHandle().destroySwapchainKHR(swapchainKHR);
	logger.logDebug("Destroyed window swapchain");
}

vk::PresentModeKHR VulkanSwapchain::choosePresentMode() {
	auto& physicalDevice = context_.getPhysicalDevice().getHandle();
	auto availableModes = physicalDevice.getSurfacePresentModesKHR(vulkanSurface_.getHandle());

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

uint32_t VulkanSwapchain::chooseImageCount() {
	auto& device = context_.getPhysicalDevice().getHandle();
	vk::SurfaceCapabilitiesKHR capabilities = device.getSurfaceCapabilitiesKHR(vulkanSurface_.getHandle());

	uint32_t result = capabilities.minImageCount + 1;

	// MaxImageCount of 0 means that there is no limit
	// If we exceed the limit, go with the limit
	if (result > capabilities.maxImageCount && capabilities.maxImageCount > 0) {
		return capabilities.maxImageCount;
	}

	return result;
}

std::vector<vk::ImageView> VulkanSwapchain::createImageViews() {
	std::vector<vk::ImageView> imageViews;

	for (auto& i : images_) {
		vk::ImageViewCreateInfo createInfo({}, i, vk::ImageViewType::e2D, format_.format,
			vk::ComponentMapping(),
			vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
		imageViews.push_back(context_.getLogicalDevice().getHandle().createImageView(createInfo));
	}
	logger.logDebug("Created swapchain image views");

	return imageViews;
}

vk::Extent2D VulkanSwapchain::chooseSwapExtent(const SdlWindow& window) {
	auto& device = context_.getPhysicalDevice().getHandle();
	vk::SurfaceCapabilitiesKHR capabilities = device.getSurfaceCapabilitiesKHR(vulkanSurface_.getHandle());

	// Special case if extent is uint32 max
	if (capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
		int width, height;
		SDL_Vulkan_GetDrawableSize(window.getHandle(), &width, &height);

		vk::Extent2D result{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		result.width = std::max(result.width, capabilities.minImageExtent.width);
		result.width = std::min(result.width, capabilities.maxImageExtent.width);

		result.height = std::max(result.height, capabilities.minImageExtent.height);
		result.height = std::min(result.height, capabilities.maxImageExtent.height);

		return result;
	}
	else {
		return capabilities.currentExtent;
	}
}

vk::SurfaceFormatKHR VulkanSwapchain::chooseSurfaceFormat() {
	auto& physicalDevice = context_.getPhysicalDevice().getHandle();
	auto availableFormats = physicalDevice.getSurfaceFormatsKHR(vulkanSurface_.getHandle());

	// Special return value that means any format is allowed
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
		return{ vk::Format::eB8G8R8A8Snorm, vk::ColorSpaceKHR::eSrgbNonlinear };
	}

	for (const auto& i : availableFormats) {
		if (i.format == vk::Format::eB8G8R8A8Snorm && i.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
			return i;
		}
	}

	return availableFormats[0];
}