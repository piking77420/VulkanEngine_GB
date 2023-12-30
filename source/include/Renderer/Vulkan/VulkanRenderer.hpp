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



	void DrawStaticMesh(Scene& scene);

	Camera* m_cam = nullptr;
	const Model* modelToDraw = nullptr;

	MakeUniform<UniformBufferObject> m_CameraUniform;

	// Draw Non Unique Class  // 
	VkDescriptorSetLayout m_DesriptorSetLayout;
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;

	VkDescriptorPool m_DescriptorPool;
	std::vector<VkCommandBuffer> m_CommandBuffers;
	// // // 


	MakeUniform<UniformBufferObject> m_UniformBufferObject;



	static inline VkCommandBuffer* currentCommandBuffer = nullptr;
};

