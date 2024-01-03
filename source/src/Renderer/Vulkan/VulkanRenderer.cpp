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
}

void VulkanRenderer::Destroy()
{
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

