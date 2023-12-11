#include "Renderer/System/RendereMesh.hpp"
#include "Renderer/MeshRenderer.h"
#include "Core/ECS/Scene.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Resource/Texture.hpp"

void RendereMesh::Begin(Scene* scene)
{
}

void RendereMesh::Update(Scene* scene)
{
}



void RendereMesh::Render(VulkanRendererData* datarenderer, Scene* scene)
{
	std::vector<Transform>* transformData = scene->GetComponentData<Transform>();

	if (transformData->empty())
		return;

	for (size_t i = 0; i < transformData->size(); i++)
	{
		Transform* transform = &transformData->at(i);



		if (!scene->HasComponent<MeshRenderer>(scene->GetEntitiesById(transform->entityId)))
			continue;


		MeshRenderer* mesh = scene->GetComponent<MeshRenderer>(transform->entityId);
		VkCommandBuffer* commandbuffer = &datarenderer->commandBuffers[datarenderer->currentFrame];



		vkCmdPushConstants(*commandbuffer, datarenderer->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Matrix4X4), &transform->Global);


		VkBuffer vertexBuffers[] = { mesh->model->vertexBuffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(*commandbuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(*commandbuffer, mesh->model->indexBuffer, 0, VK_INDEX_TYPE_UINT16);

		vkCmdBindDescriptorSets(*commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, datarenderer->pipelineLayout, 0, 1, &datarenderer->descriptorSets[datarenderer->currentFrame], 0, nullptr);


		vkCmdDrawIndexed(*commandbuffer, static_cast<uint32_t>(mesh->model->indices.size()), 1, 0, 0, 0);

	}

}

void RendereMesh::FixedUpdate(Scene* scene)
{
}

void RendereMesh::BindTexture(VulkanRendererData* datarenderer, Scene* scene,Texture* texture)
{
	return;

	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, datarenderer->descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = datarenderer->descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();




}
