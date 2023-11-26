#pragma once
#include "VulkanConfig.hpp"

namespace VkUtils 
{
	void CreateImageViews(std::vector<VkImage>& _swapChainImages, VkFormat& _swapChainImageFormat, std::vector<VkImageView>& _swapChainImageViews, VkDevice& _device);

}