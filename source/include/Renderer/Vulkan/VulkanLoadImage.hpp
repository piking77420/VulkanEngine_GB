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

		void CreateTextureDescriptorSet()
		{
			/*
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = VulkanRendererData::BindedVulkanContext->descriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &textureDescriptorSetLayout;

			if (vkAllocateDescriptorSets(VulkanRendererData::BindedVulkanContext->device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate descriptor sets!");
			}

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = textureImageView;
			imageInfo.sampler = textureSampler;

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = descriptorSet;
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);*/
		}

        void CreateTextureSampler(VulkanRendererData& vulkanRendererData);

        void CreateTextureImageView(VulkanRendererData& vulkanRendererData);


    };



	
}
