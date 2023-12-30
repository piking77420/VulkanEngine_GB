#pragma once
#include "Core/ECS/System.hpp"
#include "Renderer/Vulkan/VkContext.hpp"
#include "Renderer/Vulkan/Uniform.hpp"
class Texture;

class RendereMesh : public System 
{
public:
	void Begin(Scene* scene) override;

	void Update(Scene* scene) override;

	void Render(Scene* scene, VulkanRenderer* vkRenderer) override;

	void FixedUpdate(Scene* scene) override;

	RendereMesh();

	~RendereMesh();

	static VkDescriptorSetLayout& DescriptorSetLayout()
	{
		return m_DesriptorSetLayout;
	}

	static inline VkDescriptorPool& GetDescriptorPool()
	{
		return m_DescriptorPool;
	}



	static inline MakeUniform<UniformBufferObject> m_StaticMeshUniform;
private:
	static inline  VkDescriptorSetLayout m_DesriptorSetLayout;
	static inline VkDescriptorPool m_DescriptorPool = nullptr;


	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;
	VkRenderPass m_RenderPass;


	void CreateDescriptorPool();
	void CreateDescriptorSetLayout();
	virtual void CreateRenderPass();
	virtual void CreateGraphicPipiline();
	void InitDesciptorPool();
	  

};

