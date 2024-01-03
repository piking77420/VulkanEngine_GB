#pragma once
#include "VulkanConfig.h"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Vulkan/VBO.hpp"
#include "Renderer/Vulkan/EBO.hpp"
#include "Renderer/Vulkan/Uniform.hpp"
#include "Resource/Texture.hpp"
#include "Resource/Model.hpp"

class VkContext;

class VulkanRenderer
{
public:
	
	virtual void Init();
	virtual void Destroy();


	virtual void DrawScene(Scene* scene);

	VulkanRenderer();

	virtual	~VulkanRenderer();

	static const VkCommandBuffer const& GetCurrentCommandBuffer()
	{
		return *currentCommandBuffer;
	}


	static inline std::vector<VkDescriptorSet> m_DescriptorSets;
private:

	friend VkContext;
	virtual void DrawFrame(Scene* scene);

	virtual void CreateCommandBuffer();

	void RecordCommandBuffer(VkCommandBuffer& commandBuffer, Scene* scene);

	std::vector<VkCommandBuffer> m_CommandBuffers;

	static inline VkCommandBuffer* currentCommandBuffer = nullptr;
};

