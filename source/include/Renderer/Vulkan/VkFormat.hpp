
#pragma once
#include "VulkanConfig.hpp"


namespace VkUtils
{
    VkFormat FindSupportedFormat(VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
      

    VkFormat FindDepthFormat(VulkanRendererData* vulkanRendererData);


    static bool HasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }
}

