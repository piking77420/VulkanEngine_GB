#include "Renderer/Vulkan/VulkanRenderer.hpp"
#include "Renderer/Vulkan/VkContext.hpp"
#include "Engine.hpp"
#include "Renderer/Vulkan/Queue.hpp"
#include "Renderer/Vulkan/SwapChain.hpp"
#include "Renderer/Vulkan/VkShader.hpp"
#include "Renderer/Vulkan/Vertex.hpp"
#include "Renderer/Vulkan/Uniform.hpp"
#include "Renderer/Vulkan/Descriptor.hpp"


const std::vector<Vertex> vertices =
{
	{{-0.5f, -0.5f,0.f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f,0.f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f,0.f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f,0.f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint32_t> indices = {
	0, 1, 2, 2, 3, 0
};

void VulkanRenderer::Init()
{
	CreateCommandBuffer();
	CreateDescriptorSetLayout(m_DesriptorSetLayout);
	CreateGraphicsPipeline();
	CreateDescriptorPool(m_DescriptorPool);
	CreateDescriptorSets<UniformBufferObject>(m_DescriptorSets, m_DesriptorSetLayout, m_DescriptorPool, m_CameraUniform.GetBuffer());

	vbo.LoadVBO(vertices);
	ebo.LoadEBO(indices);
}

void VulkanRenderer::Destroy()
{
	VkDevice device = VkContext::GetDevice();


	vbo.FreeVBO();
	ebo.FreeEBO();
	vkDestroyPipeline(device, m_GraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);

}



void VulkanRenderer::DrawFrame(Scene* scene)
{
	std::uint32_t& CurrentFrame = VkContext::GetCurrentFramme();

	vkWaitForFences(VkContext::GetDevice(), 1, &VkContext::GetInFlightFence()[CurrentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(VkContext::GetDevice(), VkContext::GetSwapChain(), UINT64_MAX, VkContext::GetImageAvailableSemaphore()[CurrentFrame], VK_NULL_HANDLE, &imageIndex);

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


	RecordCommandBuffer(m_CommandBuffers[CurrentFrame],scene, imageIndex);




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

	presentInfo.pImageIndices = &imageIndex;

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


void VulkanRenderer::CreateGraphicsPipeline()
{
	using namespace VkUtils;

	auto vertShaderCode = ReadFile("shaders/vertex.spv");
	auto fragShaderCode = ReadFile("shaders/fragment.spv");

	VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();




	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();


	
	VkDescriptorSetLayout SetLayouts[] = { m_DesriptorSetLayout};
	
	VkPushConstantRange psRange;
	psRange.offset = 0;
	psRange.size = sizeof(Matrix4X4);
	psRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &m_DesriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &psRange;

	if (vkCreatePipelineLayout(VkContext::GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = m_PipelineLayout;
	pipelineInfo.renderPass = VkContext::GetRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(VkContext::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}


	vkDestroyShaderModule(VkContext::GetDevice(), fragShaderModule, nullptr);
	vkDestroyShaderModule(VkContext::GetDevice(), vertShaderModule, nullptr);
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

void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer& commandBuffer,Scene* scene, uint32_t imageIndex)
{
	
	std::uint32_t& CurrentFrame = VkContext::GetCurrentFramme();

	VkExtent2D swapChainVkExtent = VkContext::GetSwapChainVkExtent();

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	// UpdateUniform //

	m_cam.UpdateMainCamera();

	UniformBufferObject u;
	//u.model = Matrix4X4::TRS(Vector3::Zero(), Quaternion::FromEulerAngle(Vector3(0, 90 * Math::Deg2Rad, 0)), Vector3::One());
	Matrix4X4 model = Matrix4X4::TRS(Vector3::Zero(), Quaternion::FromEulerAngle(Vector3(0, 90 * Math::Deg2Rad, 0)), Vector3::One());

	u.proj = Matrix4X4::PerspectiveMatrix(90 * Math::Deg2Rad, Engine::m_Widht / Engine::m_Height, 0.01, 1000);
	u.view = Matrix4X4::LookAt(Vector3(2, 2, 2), Vector3::Zero(), Vector3::Up());
	m_CameraUniform.BindUniform(CurrentFrame, u);
	vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Matrix4X4), model.GetPtr());


	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = VkContext::GetRenderPass();
	renderPassInfo.framebuffer = VkContext::GetSwapChainFramebuffers()[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = VkContext::GetSwapChainVkExtent();

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;


	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

;


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


	VkBuffer vertexBuffers[] = { vbo.GetBuffer() };
	VkDeviceSize offsets[] = { 0 };
	//vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	//vkCmdDraw(commandBuffer, 3, 1, 0, 0);


	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, ebo.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);




	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[CurrentFrame], 0, nullptr);


	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);



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


