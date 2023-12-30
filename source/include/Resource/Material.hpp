#pragma once
#include "ResourceManager.hpp"
#include "Resource/Texture.hpp"
#include "Renderer/Vulkan/VulkanConfig.h"
#include "Renderer/Vulkan/Uniform.hpp"


class Material : public IRegisterResource<Material>
{
public:

	const Texture const* baseColor = nullptr;

	Material()
	{

	}

	~Material()
	{

	}



	virtual void LoadResource(const std::string& path) override;
	virtual void OnChange() override;
	virtual void Destroy() override;

	virtual void Bind(const VkPipelineLayout& pipelineLayout) const;

private:

	std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> m_DescriptorSets;

	void InitDescriptorSets();
	

};

