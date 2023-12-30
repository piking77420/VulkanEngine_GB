#include "Renderer/Vulkan/VulkanRenderer.hpp"
#include "Renderer/Vulkan/VkContext.hpp"
#include "Engine.hpp"
#include "Renderer/Vulkan/Queue.hpp"
#include "Renderer/Vulkan/SwapChain.hpp"
#include "Renderer/Vulkan/VkShader.hpp"
#include "Renderer/Vulkan/Vertex.hpp"
#include "Renderer/Vulkan/Uniform.hpp"
#include "Renderer/Vulkan/Descriptor.hpp"
#include "Resource/ResourceManager.hpp"
#include "Renderer/Vulkan/GraphicPipeline.hpp"
#include <Renderer/MeshRenderer.h>



void VulkanRenderer::Init()
{
	// Create Commadn Buffer
	CreateCommandBuffer();

	// Create Inputs Layout 
	// what will your shader nned in input Uniform texture etc
	CreateDescriptorSetLayout(&m_DesriptorSetLayout);

	// Create The GraphicsPipeline from shader
	CreateGraphicsPipeline(m_DesriptorSetLayout,VkContext::GetRenderPass(),&m_PipelineLayout,&m_GraphicsPipeline);

	// Create descriptor pool 
	CreateDescriptorPool(m_DescriptorPool);
	
	m_CameraUniform.Init();

	Texture* texture = ResourceManager::GetResource<Texture>("viking_room");
	CreateDescriptorSets<UniformBufferObject>(m_DescriptorSets, m_DesriptorSetLayout, m_DescriptorPool, m_CameraUniform,*texture);
	
	modelToDraw = ResourceManager::GetResource<Model>("viking_room");

	
	m_cam = Camera::SetMainCamera();
}

void VulkanRenderer::Destroy()
{
	VkDevice device = VkContext::GetDevice();
	m_CameraUniform.Destroy();
	vkDestroyPipeline(device, m_GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);

}



void VulkanRenderer::DrawFrame(Scene* scene)
{
	std::uint32_t& CurrentFrame = VkContext::GetCurrentFramme();

	vkWaitForFences(VkContext::GetDevice(), 1, &VkContext::GetInFlightFence()[CurrentFrame], VK_TRUE, UINT64_MAX);

	VkResult result = vkAcquireNextImageKHR(VkContext::GetDevice(), VkContext::GetSwapChain(), UINT64_MAX, VkContext::GetImageAvailableSemaphore()[CurrentFrame], VK_NULL_HANDLE, &VkContext::ImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		VkContext::RecreateSwapChain();
		Engine::imguivulkan.OnResizeFrammebuffer();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}


	vkResetFences(VkContext::GetDevice(), 1, &VkContext::GetInFlightFence()[CurrentFrame]);

	

	RecordCommandBuffer(m_CommandBuffers[CurrentFrame],scene);


	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { VkContext::GetImageAvailableSemaphore()[VkContext::GetCurrentFramme()] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffers[VkContext::GetCurrentFramme()];

	VkSemaphore signalSemaphores[] = { VkContext::GetRenderFinishedSemaphore()[VkContext::GetCurrentFramme()]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(VkContext::GetGraphicQueue(), 1, &submitInfo, VkContext::GetInFlightFence()[VkContext::GetCurrentFramme()]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { VkContext::GetSwapChain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &VkContext::ImageIndex;

	result = vkQueuePresentKHR(VkContext::GetPresentQueue(), &presentInfo);


	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || VkContext::FramebufferResized) 
	{
		VkContext::FramebufferResized = false;
		VkContext::RecreateSwapChain();
		Engine::imguivulkan.OnResizeFrammebuffer();

	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	VkContext::NextFrame();

}

void VulkanRenderer::DrawScene(Scene* scene)
{
	if (!scene)
		return;

	DrawFrame(scene);
}

VulkanRenderer::VulkanRenderer()
{
	Init();
}

VulkanRenderer::~VulkanRenderer()
{
	Destroy();
}



void VulkanRenderer::CreateCommandBuffer()
{
	m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);


	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = VkContext::GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

	if (vkAllocateCommandBuffers(VkContext::GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer& commandBuffer,Scene* scene)
{
	currentCommandBuffer = &commandBuffer;
		

	std::uint32_t& CurrentFrame = VkContext::GetCurrentFramme();

	VkExtent2D swapChainVkExtent = VkContext::GetSwapChainVkExtent();

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}


	

	/*

	//vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Matrix4X4), model.GetPtr());


	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = VkContext::GetRenderPass();
	renderPassInfo.framebuffer = VkContext::GetSwapChainFramebuffers()[VkContext::ImageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = VkContext::GetSwapChainVkExtent();

	// Clear Value
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();


	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

	


	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainVkExtent.width;
	viewport.height = (float)swapChainVkExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainVkExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	
	*/

	//DrawStaticMesh(*scene);
	scene->Render(this);

	
	ImGui::Begin("sqdqs");
	ImGui::End();



	ImGuiIO& io = ImGui::GetIO();
	ImGui::Render();


	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

	
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}





}

void VulkanRenderer::DrawStaticMesh(Scene& scene)
{
	
	std::vector<MeshRenderer>* meshRenderer = scene.GetComponentData<MeshRenderer>();
	std::vector<Transform>* transforms = scene.GetComponentData<Transform>();

	for (int i = 0; i < meshRenderer->size(); i++)
	{
		UniformBufferObject u;

		u.proj = Matrix4X4::PerspectiveMatrix(90 * Math::Deg2Rad, Engine::m_Widht / Engine::m_Height, 0.1, 10.f);

		vkCmdPushConstants(GetCurrentCommandBuffer(), m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Matrix4X4), transforms->at(i).Global.GetPtr());

		u.view = m_cam->GetLookMatrix();

		m_CameraUniform.BindUniform(u);

		const Model& model = *meshRenderer->at(i).model;

		modelToDraw->Vbo.BindVBO();
		modelToDraw->Ebo.BindEBO();


		vkCmdBindDescriptorSets(GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[VkContext::GetCurrentFramme()], 0, nullptr);

		vkCmdDrawIndexed(GetCurrentCommandBuffer(), static_cast<uint32_t>(modelToDraw->indices.size()), 1, 0, 0, 0);

	}
	
}

