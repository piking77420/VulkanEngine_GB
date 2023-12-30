#include "Renderer/Vulkan/VBO.hpp"
#include "Renderer/Vulkan/Buffer.hpp"
#include "Renderer/Vulkan/VkContext.hpp"
#include "Renderer/Vulkan/Vertex.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"

using namespace VkUtils;

    



void VBO::BindVBO() const
{
    VkBuffer buff{ GetBuffer() };
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(VulkanRenderer::GetCurrentCommandBuffer(), 0, 1, &buff, offsets);
}

void VBO::LoadVBO(const std::vector<Vertex>& verticies)
{
    const VkDevice& device = VkContext::GetDevice();

    VkDeviceSize bufferSize = sizeof(Vertex) * verticies.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, verticies.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

    CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

}

void VBO::FreeVBO()
{
    const VkDevice& device = VkContext::GetDevice();

    vkDestroyBuffer(device, m_VertexBuffer, nullptr);
    vkFreeMemory(device, m_VertexBufferMemory, nullptr);
}

void VBO::operator=(const VBO& vbo)
{
    this->m_VertexBuffer = vbo.m_VertexBuffer;
    this->m_VertexBufferMemory = vbo.m_VertexBufferMemory;

}
