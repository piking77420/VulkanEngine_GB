#pragma once
#include "ResourceManager.hpp"
#include "Renderer/Vulkan/VkTexture.hpp"

enum class TextureType
{
	TEXTURE,
	NORMALMAP,
	PARRALAXMAP,

};


class Texture : public IRegisterResource<Texture>
{
public:
	virtual ~Texture() override 
	{
		
	}

	virtual void Destroy() override;

	



	// Inherited via IRegisterResource
	void LoadResource(const std::string& path) override;

	VkImage m_TextureImage;
	VkDeviceMemory m_TextureImageMemory;
	VkImageView m_TextureImageView;
private:

	

	// Hérité via IRegisterResource
	virtual void OnChange() override;

};

