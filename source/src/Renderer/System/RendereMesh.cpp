#include "Renderer/System/RendereMesh.hpp"
#include "Renderer/MeshRenderer.h"
#include "Core/ECS/Scene.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"

void RendereMesh::Begin(Scene* scene)
{
}

void RendereMesh::Update(Scene* scene)
{
}

void RendereMesh::UpdateRender(VulkanRendererData* datarenderer, Scene* scene)
{
	std::vector<MeshRenderer>& meshesRender = *scene->GetComponentData<MeshRenderer>();
	std::vector<Transform>& transform = *scene->GetComponentData<Transform>();
	/*
	UniformBufferObject ubo;
	ubo.view = Matrix4X4::LookAt(Vector3(2.0f, 2.0f, 2.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	ubo.proj = Matrix4X4::PerspectiveMatrix(Math::Deg2Rad * 45.0f, datarenderer->swapChainExtent.width / (float)datarenderer->swapChainExtent.height, 0.1f, 10.0f);
	*/
	for (int i = 0; i < meshesRender.size(); i++)
	{
		MeshRenderer& mesh = meshesRender[i];


		if (!mesh.model)
			continue;

		/*
		Transform* transform = scene->GetComponent<Transform>(mesh.entityId);

		// Update Uniform
		ubo.model = transform->Global;
		memcpy(datarenderer->uniformBuffersMapped[datarenderer->currentFrame], &ubo, sizeof(ubo));
		*/
	}
}

void RendereMesh::Render(VulkanRendererData* datarenderer, Scene* scene)
{
	
	
	std::vector<MeshRenderer>& meshesRender = *scene->GetComponentData<MeshRenderer>();
	std::vector<Transform>& transform = *scene->GetComponentData<Transform>();
	/*
	UniformBufferObject ubo;
	ubo.view = Matrix4X4::LookAt(Vector3(2.0f, 2.0f, 2.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	ubo.proj = Matrix4X4::PerspectiveMatrix(Math::Deg2Rad * 45.0f, datarenderer->swapChainExtent.width / (float)datarenderer->swapChainExtent.height, 0.1f, 10.0f);
	*/
	for(int i = 0 ; i < meshesRender.size(); i++)
	{
		MeshRenderer& mesh = meshesRender[i];


		if (!mesh.model)
			continue;
		
		/*
		Transform* transform = scene->GetComponent<Transform>(mesh.entityId);
		
		// Update Uniform 
		ubo.model = transform->Global;
		memcpy(datarenderer->uniformBuffersMapped[datarenderer->currentFrame], &ubo, sizeof(ubo));
		*/


		
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
