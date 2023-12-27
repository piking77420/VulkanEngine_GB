#pragma once
#include "VulkanConfig.h"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/Vulkan/VBO.hpp"
#include "Renderer/Vulkan/EBO.hpp"
#include "Renderer/Vulkan/Uniform.hpp"

class VkContext;

class VulkanRenderer
{
public:
	
	virtual void Init();
	virtual void Destroy();


	virtual void DrawScene(Scene* scene);

	VulkanRenderer();
	virtual	~VulkanRenderer();

private:

	friend VkContext;
	virtual void DrawFrame(Scene* scene);

	void CreateGraphicsPipeline();
	virtual void CreateCommandBuffer();

	void RecordCommandBuffer(VkCommandBuffer& commandBuffer, Scene* scene, uint32_t imageIndex);
	Camera m_cam;

	MakeUniform<UniformBufferObject> m_CameraUniform;

	VkDescriptorSetLayout m_DesriptorSetLayout;
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;

	std::vector<VkCommandBuffer> m_CommandBuffers;
	MakeUniform<UniformBufferObject> m_UniformBufferObject;


	VkDescriptorPool m_DescriptorPool;
	std::vector<VkDescriptorSet> m_DescriptorSets;

	VBO vbo;
	EBO ebo;

};

