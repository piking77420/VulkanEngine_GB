#pragma once
#include "VulkanConfig.h"
#include "VkContext.hpp"
#include "Buffer.hpp"


template<class T>
class MakeUniform
{
public:


	void BindUniform(const T& updateUniform)
	{
		std::memcpy(m_uniformBuffersMapped[VkContext::GetCurrentFramme()], &updateUniform, sizeof(T));
	}

	void Init()
	{
		VkDeviceSize bufferSize = sizeof(T);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			VkUtils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);

			vkMapMemory(VkContext::GetDevice(), m_uniformBuffersMemory[i], 0, bufferSize, 0, &m_uniformBuffersMapped[i]);
		}
	}

	void Destroy()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyBuffer(VkContext::GetDevice(), m_uniformBuffers[i], nullptr);
			vkFreeMemory(VkContext::GetDevice(), m_uniformBuffersMemory[i], nullptr);
		}
	}

	MakeUniform()
	{
		
	}

	~MakeUniform()
	{
		

	}

	std::array<VkBuffer, MAX_FRAMES_IN_FLIGHT>& GetBuffer()
	{
		return m_uniformBuffers;
	}


	const std::uint32_t SIZE = sizeof(T);

private:

	std::array<VkBuffer, MAX_FRAMES_IN_FLIGHT> m_uniformBuffers;
	std::array<VkDeviceMemory, MAX_FRAMES_IN_FLIGHT> m_uniformBuffersMemory;
	std::array<void*, MAX_FRAMES_IN_FLIGHT> m_uniformBuffersMapped;
};


void CreateDescriptorSetLayout(VkDescriptorSetLayout* descriptorSetLayout);
