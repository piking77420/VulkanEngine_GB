#include "VulkanConfig.hpp"
#include "Vertex.hpp"

namespace VkUtils
{
    static uint32_t FindMemoryType(VkPhysicalDevice& physicalDevice,uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }


	static void CreateBuffer(VulkanRendererData& data,VkDeviceSize size, VkBufferUsageFlags usage,VkMemoryPropertyFlags properties,VkBuffer* buffer,VkDeviceMemory* bufferMemroy)
	{
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(data.device, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
            throw std::runtime_error("echec de la creation d'un buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(data.device, *buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(data.physicalDevice,memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(data.device, &allocInfo, nullptr, bufferMemroy) != VK_SUCCESS) {
            throw std::runtime_error("fail of memory allocation !");
        }

        vkBindBufferMemory(data.device, *buffer, *bufferMemroy, 0);
	}

    

    static void CopyBuffer(VulkanRendererData& VkrData,VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = VkrData.commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(VkrData.device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(VkrData.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(VkrData.graphicsQueue);

        vkFreeCommandBuffers(VkrData.device, VkrData.commandPool, 1, &commandBuffer);
    }

    static void CreateVertexBuffer(VulkanRendererData& VkrData, std::vector<Vertex>& vertices)
    {
        VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(VkrData, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

        void* data;
        vkMapMemory(VkrData.device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(VkrData.device, stagingBufferMemory);

        CreateBuffer(VkrData, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &VkrData.vertexBuffer, &VkrData.vertexBufferMemory);

        CopyBuffer(VkrData, stagingBuffer, VkrData.vertexBuffer, bufferSize);

        vkDestroyBuffer(VkrData.device, stagingBuffer, nullptr);
        vkFreeMemory(VkrData.device, stagingBufferMemory, nullptr);
    }

   static void CreateIndexBuffer(VulkanRendererData& VkrData, std::vector<uint16_t >& indices)
   {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        CreateBuffer(VkrData,bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

        void* data;
        vkMapMemory(VkrData.device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(VkrData.device, stagingBufferMemory);

        CreateBuffer(VkrData,bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &VkrData.indexBuffer, &VkrData.indexBufferMemory);

        CopyBuffer(VkrData,stagingBuffer, VkrData.indexBuffer, bufferSize);

        vkDestroyBuffer(VkrData.device, stagingBuffer, nullptr);
        vkFreeMemory(VkrData.device, stagingBufferMemory, nullptr);
   }
}