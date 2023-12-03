#include "Renderer/Vulkan/ImageView.hpp"

void VkUtils::CreateImageViews(VulkanRendererData& vulkanRendererData,std::vector<VkImage>& _swapChainImages, VkFormat& _swapChainImageFormat, std::vector<VkImageView>& _swapChainImageViews,VkDevice& _device)
{
	_swapChainImageViews.resize(_swapChainImages.size());

	for (uint32_t i = 0; i < _swapChainImages.size(); i++) {
		_swapChainImageViews[i] = CreateImageView(vulkanRendererData,_swapChainImages[i], _swapChainImageFormat);
	}
	CORE_LOG_SUCCESS("Succeed to vkCreateImageView \n");

}

VkImageView VkUtils::CreateImageView(VulkanRendererData& vulkanRendererData, VkImage image, VkFormat format)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(vulkanRendererData.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}



