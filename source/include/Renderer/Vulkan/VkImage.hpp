#pragma once
#include "VulkanConfig.hpp"


namespace VkUtils
{
    void CreateVKImage(VulkanRendererData& vulkanRendererData, int32_t width, uint32_t height,
        VkFormat format, VkImageTiling tiling,
        VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
        VkImage& image, VkDeviceMemory& imageMemory);

    VkImageView CreateImageView(VulkanRendererData& vulkanRendererData, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);



   void CreateImageViews(VulkanRendererData& vulkanRendererData, std::vector<VkImage>& _swapChainImages, VkFormat& _swapChainImageFormat, std::vector<VkImageView>& _swapChainImageViews, VkDevice& _device);


}
