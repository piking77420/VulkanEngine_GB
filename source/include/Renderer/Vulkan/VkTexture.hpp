#pragma once
#include "VulkanConfig.h"

void CreateTextureImage(const char* path,VkImage& textureImage, VkDeviceMemory& textureImageMemory);

void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);


void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);