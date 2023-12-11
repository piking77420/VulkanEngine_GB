#include "ViewPort/Hierachy.hpp"
#include "Physics/Transform.hpp"
#include "Core/ECS/Scene.hpp"
#include <Imgui/imgui.h>
#include"Renderer/Vulkan/VulkanHeader.hpp"

void Hierachy::createViewportImage()
{
}

void Hierachy::createViewportImageViews()
{
	m_ViewportImages.resize(m_SwapChainImages.size());
	m_DstImageMemory.resize(m_SwapChainImages.size());

	for (uint32_t i = 0; i < m_SwapChainImages.size(); i++)
	{
		// Create the linear tiled destination image to copy to and to read the memory from
		VkImageCreateInfo imageCreateCI{};
		imageCreateCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateCI.imageType = VK_IMAGE_TYPE_2D;
		// Note that vkCmdBlitImage (if supported) will also do format conversions if the swapchain color format would differ
		imageCreateCI.format = VK_FORMAT_B8G8R8A8_SRGB;
		imageCreateCI.extent.width = m_SwapChainExtent.width;
		imageCreateCI.extent.height = m_SwapChainExtent.height;
		imageCreateCI.extent.depth = 1;
		imageCreateCI.arrayLayers = 1;
		imageCreateCI.mipLevels = 1;
		imageCreateCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateCI.tiling = VK_IMAGE_TILING_LINEAR;
		imageCreateCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		// Create the image
		// VkImage dstImage;
		vkCreateImage(m_Device, &imageCreateCI, nullptr, &m_ViewportImages[i]);
		// Create memory to back up the image
		VkMemoryRequirements memRequirements;
		VkMemoryAllocateInfo memAllocInfo{};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		// VkDeviceMemory dstImageMemory;
		vkGetImageMemoryRequirements(m_Device, m_ViewportImages[i], &memRequirements);
		memAllocInfo.allocationSize = memRequirements.size;
		// Memory must be host visible to copy from
		memAllocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		vkAllocateMemory(m_Device, &memAllocInfo, nullptr, &m_DstImageMemory[i]);
		vkBindImageMemory(m_Device, m_ViewportImages[i], m_DstImageMemory[i], 0);

		VkCommandBuffer copyCmd = beginSingleTimeCommands(m_ViewportCommandPool);

		insertImageMemoryBarrier(
			copyCmd,
			m_ViewportImages[i],
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

		endSingleTimeCommands(copyCmd, m_ViewportCommandPool);
	}

}

void Hierachy::Begin(Scene* scene)
{
}

void Hierachy::Update(Scene* scene)
{
}


void Hierachy::Render(VulkanRendererData* datarenderer, Scene* scene)
{

	std::vector<Transform>* transform = scene->GetComponentData<Transform>();
	
	if(ImGui::Begin("Hierachy"))
	{
		

		for (std::vector<Transform>::iterator it = transform->begin(); it != transform->end(); it++)
		{
			Transform& ptr = *it;

			ImGui::CollapsingHeader(("entity " + std::to_string(ptr.entityId)).c_str());
			ImGui::PushID(ptr.entityId);

			ImGui::DragFloat3("Pos", ptr.pos.SetPtr());
			ImGui::DragFloat3("rotation", ptr.rotation.SetPtr());

			ImGui::DragFloat3("scale", ptr.scale.SetPtr());

			ImGui::PopID();


		}
		ImGui::End();
	}



}
