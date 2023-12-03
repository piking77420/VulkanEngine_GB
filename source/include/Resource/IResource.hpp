#pragma once

class VulkanRendererData;

class IResource
{
public:
	virtual ~IResource() {};
	virtual void Destroy(VulkanRendererData& data) = 0 ;
	virtual void LoadResource(const std::string& path, VulkanRendererData& _vulkanRendererData) = 0;

};


