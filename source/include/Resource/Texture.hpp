#pragma once
#include "ResourceManager.hpp"
#include "Renderer/Vulkan/VulkanLoadImage.hpp"


class Texture : public VkClass::VkTexture, public IRegisterResource<Texture>
{
public:
	virtual ~Texture() override 
	{
		
	}

	virtual void Destroy(VulkanRendererData& data)
	{
		VkTexture::Destroy(data);
	}


	// Inherited via IRegisterResource
	void LoadResource(const std::string& path, VulkanRendererData& _vulkanRendererData) override;

};

