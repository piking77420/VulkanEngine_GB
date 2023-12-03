#pragma once
#include "VulkanConfig.hpp"



namespace VkClass
{
 
    class VkTexture
    {
    public :

        void LoadImage(const std::string& path, VulkanRendererData& vulkanRendererData);
        
        

        void Destroy(VulkanRendererData& vulkanRendererData)
        {
            vkDestroySampler(vulkanRendererData.device, textureSampler, nullptr);
            vkDestroyImageView(vulkanRendererData.device, textureImageView, nullptr);

            vkDestroyImage(vulkanRendererData.device, textureImage, nullptr);
            vkFreeMemory(vulkanRendererData.device, textureImageMemory, nullptr);

        }
      


    protected:
        void CreateVKImage(VulkanRendererData& vulkanRendererData, int32_t width, uint32_t height,
            VkFormat format, VkImageTiling tiling, 
            VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
            VkImage& image, VkDeviceMemory& imageMemory);

        void CreateTextureImageView(VulkanRendererData& vulkanRendererData);

        void CreateTextureSampler(VulkanRendererData& vulkanRendererData);
       


        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;

    };



	
}
