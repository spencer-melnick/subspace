#include "swapchain.hpp"

// STL includes
#include <limits>
#include <utility>

// Library includes
#include <SDL2/SDL_vulkan.h>

// Project includes
#include "engine/util/logger.hpp"

using namespace std;
using namespace subspace;

Swapchain::Swapchain(const Context& context, const SdlWindow& window, const vk::SurfaceKHR& surface) :
	context_(context), vulkanSurface_(surface)
{
	chooseSurfaceFormat();
	choosePresentMode();
	chooseSwapExtent(window);
	uint32_t numImages = chooseImageCount();

	vk::SwapchainCreateInfoKHR createInfo(
	{}, vulkanSurface_, numImages, format_.format,
		format_.colorSpace, extent_, 1, vk::ImageUsageFlagBits::eColorAttachment,
		vk::SharingMode::eExclusive, 0, nullptr, vk::SurfaceTransformFlagBitsKHR::eIdentity,
		vk::CompositeAlphaFlagBitsKHR::eOpaque, mode_, true);

	handle_ = context_.getLogicalDevice().createSwapchainKHRUnique(createInfo);
	images_ = context_.getLogicalDevice().getSwapchainImagesKHR(*handle_);
	images_.resize(numImages);
	createImageViews();
	createRenderPass();
	createFramebuffers();

	logger.logVerbose("Created swapchain");
}

Swapchain::operator const vk::SwapchainKHR&() const {
	return *handle_;
}

const vk::SurfaceFormatKHR& Swapchain::getFormat() const {
	return format_;
}

const vk::Extent2D& Swapchain::getExtent() const {
	return extent_;
}

const std::vector<vk::UniqueImageView>& Swapchain::getImageViews() const {
	return imageViews_;
}

void Swapchain::choosePresentMode() {
	auto& physicalDevice = context_.getChosenPhysicalDevice();
	auto availableModes = physicalDevice->getSurfacePresentModesKHR(vulkanSurface_);

	// Try for mailbox present mode - triple buffering!
	for (const auto& i : availableModes) {
		if (i == vk::PresentModeKHR::eMailbox) {
			mode_ = i;
			return;
		}
	}

	// Immediate mode has best support
	for (const auto& i : availableModes) {
		if (i == vk::PresentModeKHR::eImmediate) {
			mode_ = i;
			return;
		}
	}

	// FIFO is guaranteed, but not always well supported
	mode_ = vk::PresentModeKHR::eFifo;
}

uint32_t Swapchain::chooseImageCount() {
	auto& device = context_.getChosenPhysicalDevice();
	vk::SurfaceCapabilitiesKHR capabilities = device->getSurfaceCapabilitiesKHR(vulkanSurface_);

	uint32_t result = capabilities.minImageCount + 1;

	// MaxImageCount of 0 means that there is no limit
	// If we exceed the limit, go with the limit
	if (result > capabilities.maxImageCount && capabilities.maxImageCount > 0) {
		return capabilities.maxImageCount;
	}

	return result;
}

void Swapchain::createImageViews() {
	for (auto& i : images_) {
		vk::ImageViewCreateInfo createInfo({}, i, vk::ImageViewType::e2D, format_.format,
			vk::ComponentMapping(),
			vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
		imageViews_.push_back(move(context_.getLogicalDevice().createImageViewUnique(createInfo)));
	}
}

void Swapchain::chooseSwapExtent(const SdlWindow& window) {
	auto& device = context_.getChosenPhysicalDevice();
	vk::SurfaceCapabilitiesKHR capabilities = device->getSurfaceCapabilitiesKHR(vulkanSurface_);

	// Special case if extent is uint32 max
	if (capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
		int width, height;
		SDL_Vulkan_GetDrawableSize(window, &width, &height);

		vk::Extent2D result{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		result.width = std::max(result.width, capabilities.minImageExtent.width);
		result.width = std::min(result.width, capabilities.maxImageExtent.width);

		result.height = std::max(result.height, capabilities.minImageExtent.height);
		result.height = std::min(result.height, capabilities.maxImageExtent.height);

		extent_ = result;
	}
	else {
		extent_ = capabilities.currentExtent;
	}
}

void Swapchain::chooseSurfaceFormat() {
	auto& physicalDevice = context_.getChosenPhysicalDevice();
	auto availableFormats = physicalDevice->getSurfaceFormatsKHR(vulkanSurface_);

	// Special return value that means any format is allowed
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
		format_ = vk::SurfaceFormatKHR{vk::Format::eB8G8R8A8Snorm, vk::ColorSpaceKHR::eSrgbNonlinear};
		return;
	}

	for (const auto& i : availableFormats) {
		if (i.format == vk::Format::eB8G8R8A8Snorm &&
			i.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			format_ = i;
			return;
		}
	}

	format_ = availableFormats[0];
}

void Swapchain::createRenderPass() {
	vk::AttachmentDescription colorAttachment({}, format_.format,
        vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);
        
    vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 1, &colorAttachmentRef);
    vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        {}, vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);

    vk::RenderPassCreateInfo createInfo({}, 1, &colorAttachment, 1, &subpass, 1, &dependency);
    renderPass_ = context_.getLogicalDevice().createRenderPassUnique(createInfo);
}

void Swapchain::createFramebuffers() {
	for (auto& i : imageViews_) {
		auto framebuffer = context_.getLogicalDevice().createFramebufferUnique({
			{}, *renderPass_, 1, &(*i),
			extent_.width, extent_.height, 0
		});

		framebuffers_.push_back(move(framebuffer));
	}
}