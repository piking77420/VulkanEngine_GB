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
      


        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;

    protected:



        void CreateTextureSampler(VulkanRendererData& vulkanRendererData);

        void CreateTextureImageView(VulkanRendererData& vulkanRendererData);


    };



	
}
