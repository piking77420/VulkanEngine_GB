#pragma once
#include "Renderer/Vulkan/Vertex.hpp"
#include "Resource/ResourceManager.hpp" 

class Model : public IRegisterResource<Model>
{
public:


	virtual void LoadResource(const std::string& path, VulkanRendererData& _vulkanRendererData) override;

	virtual void Destroy(VulkanRendererData& data) override ;


	Model() {};
	~Model() {};

	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;


	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

};

