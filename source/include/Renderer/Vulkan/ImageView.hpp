#pragma once
#include "VulkanConfig.hpp"

namespace VkUtils 
{
	void CreateImageViews(VulkanRendererData& vulkanRendererData,std::vector<VkImage>& _swapChainImages, VkFormat& _swapChainImageFormat, std::vector<VkImageView>& _swapChainImageViews, VkDevice& _device);


	VkImageView CreateImageView(VulkanRendererData& vulkanRendererData, VkImage image, VkFormat format);
 

}