#include "Renderer/Vulkan/EBO.hpp"
#include "Renderer/Vulkan/Buffer.hpp"
#include "Renderer/Vulkan/VkContext.hpp"
#include "Renderer/Vulkan/Vertex.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"

using namespace VkUtils;

void EBO::BindEBO() const
{
    vkCmdBindIndexBuffer(VulkanRenderer::GetCurrentCommandBuffer(), GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

}   

void EBO::LoadEBO(const std::vector<uint32_t>& indicies)
{
    const VkDevice& device = VkContext::GetDevice();

    VkDeviceSize bufferSize = sizeof(indicies[0]) * indicies.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indicies.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

    CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void EBO::LoadEBO(const std::vector<uint16_t>& indicies)
{
    const VkDevice& device = VkContext::GetDevice();

    VkDeviceSize bufferSize = sizeof(uint16_t) * indicies.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indicies.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

    CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}


void EBO::FreeEBO()
{
    const VkDevice& device = VkContext::GetDevice();

    vkDestroyBuffer(device, m_IndexBuffer, nullptr);
    vkFreeMemory(device, m_IndexBufferMemory, nullptr);
}

void EBO::operator=(const EBO& vbo)
{
}
