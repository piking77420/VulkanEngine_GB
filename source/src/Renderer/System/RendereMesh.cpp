#include "Renderer/System/RendereMesh.hpp"
#include "Renderer/MeshRenderer.h"
#include "Core/ECS/Scene.hpp"

void RendereMesh::Begin(Scene* scene)
{
}

void RendereMesh::Update(Scene* scene)
{
}

void RendereMesh::Render(VulkanRendererData* datarenderer, Scene* scene)
{
	std::vector<MeshRenderer>& meshesRender = *scene->GetComponentData<MeshRenderer>();


	for(MeshRenderer& mesh : meshesRender)
	{
		if (!mesh.model)
			continue;
		
		VkCommandBuffer* commandbuffer = &datarenderer->commandBuffers[datarenderer->currentFrame];
		
		VkBuffer vertexBuffers[] = { mesh.model->vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(*commandbuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(*commandbuffer, mesh.model->indexBuffer, 0, VK_INDEX_TYPE_UINT16);

		vkCmdBindDescriptorSets(*commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, datarenderer->pipelineLayout, 0, 1, &datarenderer->descriptorSets[datarenderer->currentFrame], 0, nullptr);

		vkCmdDrawIndexed(*commandbuffer, static_cast<uint32_t>(mesh.model->indices.size()), 1, 0, 0, 0);
		

	}

}

void RendereMesh::FixedUpdate(Scene* scene)
{
}
