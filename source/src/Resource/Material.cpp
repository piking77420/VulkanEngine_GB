#include "Resource/Material.hpp"
#include "Renderer/Vulkan/VkTexture.hpp"
#include "Renderer/Vulkan/VkContext.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"
#include "Renderer/System/RendereMesh.hpp"

void Material::LoadResource(const std::string& path)
{

}

void Material::OnChange()
{
    if (!baseColor)
        return;
	
	InitDescriptorSets();
}
void Material::Bind(const VkPipelineLayout& pipelineLayout) const
{
    if (!baseColor)
        return;

	

	vkCmdBindDescriptorSets(VulkanRenderer::GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_DescriptorSets[VkContext::GetCurrentFramme()], 0, nullptr);
}

void Material::InitDescriptorSets()
{


	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, RendereMesh::DescriptorSetLayout());
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = RendereMesh::GetDescriptorPool();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(VkContext::GetDevice(), &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = RendereMesh::m_StaticMeshUniform.GetBuffer()[i];
		bufferInfo.offset = 0;
		bufferInfo.range = RendereMesh::m_StaticMeshUniform.SIZE;

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = baseColor->m_TextureImageView;
		imageInfo.sampler = VkContext::GetGetTextureSampler();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_DescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = m_DescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;


		vkUpdateDescriptorSets(VkContext::GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

}







void Material::Destroy()
{
}
