#include "Renderer/Vulkan/Depth.hpp"
#include "Renderer/Vulkan/VkFormat.hpp"
#include "Renderer/Vulkan/VkTexture.hpp"
#include "Renderer/Vulkan/VkContext.hpp"

using namespace VkUtils;



void VkDepth::CreateDepthRessource()
{
    VkFormat depthFormat = FindDepthFormat();

    const VkExtent2D& extent = VkContext::GetSwapChainVkExtent();

    CreateImage(extent.width, extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    depthImageView = CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void VkDepth::DestroyDepthRessource()
{
    const VkDevice& device = VkContext::GetDevice();
    vkDestroyImageView(device, depthImageView, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    vkFreeMemory(device, depthImageMemory, nullptr);
}
