#include "..\..\..\include\Renderer\ImguiImplement\ImguiVulkan.hpp"
#include "Renderer/Vulkan/VkContext.hpp"
#include "Renderer/Vulkan/Queue.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>	

#define MAX_FRAMES_IN_FLIGHT 2 

void ImguiVulkan::InitImgui(GLFWwindow* window)
{
	InitImgui();

	
	CreateImGuiRenderPass();
	CreateImguiCommandPool(&m_CommandPool);
	CreateImGuiCommandBuffers();
	CreateImGuiFramebuffers();
	

	QueueFamilyIndices queueFamilyIndices = VkUtils::FindQueueFamilies(VkContext::GetPhysicalDevice());

	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = VkContext::GetInstance();
	init_info.PhysicalDevice = VkContext::GetPhysicalDevice();
	init_info.Device = VkContext::GetDevice();
	init_info.QueueFamily = queueFamilyIndices.graphicsFamily.value();
	init_info.Queue = VkContext::GetGraphicQueue();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = m_ImGuiDescriptorPool;
	init_info.Subpass = 0;
	init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
	init_info.ImageCount = MAX_FRAMES_IN_FLIGHT ;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.Allocator = nullptr;
	init_info.CheckVkResultFn = nullptr;
	ImGui_ImplVulkan_Init(&init_info, m_ImguiRenderPass);

	for (uint32_t i = 0; i < m_Dset.size(); i++)
		m_Dset[i] = ImGui_ImplVulkan_AddTexture(VkContext::GetGetTextureSampler(), VkContext::GetSwapChainImageViews()[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

}

void ImguiVulkan::DestroyImgui()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	



	vkDestroyDescriptorPool(VkContext::GetDevice(), m_ImGuiDescriptorPool, nullptr);


	for (size_t i = 0; i < m_ImGuiFramebuffers.size(); i++)
	{
		vkDestroyFramebuffer(VkContext::GetDevice(), m_ImGuiFramebuffers[i], nullptr);
	}

	vkDestroyCommandPool(VkContext::GetDevice(), m_CommandPool, nullptr);
	vkDestroyRenderPass(VkContext::GetDevice(), m_ImguiRenderPass, nullptr);


}

void ImguiVulkan::NewFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


}

void ImguiVulkan::EndFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Render();


	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ImguiVulkan::ImguiRender()
{
	
	ImGui::Begin("Viewport");

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	ImGui::Image(m_Dset[VkContext::GetCurrentFramme()], ImVec2{ viewportPanelSize.x, viewportPanelSize.y });

	ImGui::End();



	EndFrame();

	vkResetCommandBuffer(m_ImGuiCommandBuffers[VkContext::GetCurrentFramme()], /*VkCommandBufferResetFlagBits*/ 0);
	

	vkResetCommandPool(VkContext::GetDevice(), m_CommandPool, 0);
	VkCommandBufferBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(m_ImGuiCommandBuffers[VkContext::GetCurrentFramme()], &info);

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_ImguiRenderPass;
	renderPassInfo.framebuffer = m_ImGuiFramebuffers[VkContext::GetCurrentFramme()];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = VkContext::GetSwapChainVkExtent();
	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	vkCmdBeginRenderPass(m_ImGuiCommandBuffers[VkContext::GetCurrentFramme()], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);



	// Record dear imgui primitives into command buffer
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_ImGuiCommandBuffers[VkContext::GetCurrentFramme()]);

	vkCmdEndRenderPass(m_ImGuiCommandBuffers[VkContext::GetCurrentFramme()]);
	vkEndCommandBuffer(m_ImGuiCommandBuffers[VkContext::GetCurrentFramme()]);
	
}

void ImguiVulkan::OnResizeFrammebuffer()
{
	ImGui_ImplVulkan_SetMinImageCount(MAX_FRAMES_IN_FLIGHT);
	CreateImGuiRenderPass();
	CreateImGuiFramebuffers();
	CreateImGuiCommandBuffers();
}



void ImguiVulkan::InitImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	{
		VkDescriptorPoolSize pool_sizes[] =
		{
				{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
				{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
				{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
				{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
				{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000} };

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		if (vkCreateDescriptorPool(VkContext::GetDevice(), &pool_info, nullptr, &m_ImGuiDescriptorPool) != VK_SUCCESS)
			throw std::runtime_error("Create DescriptorPool for m_ImGuiDescriptorPool failed!");
	}






}

void ImguiVulkan::CreateImGuiRenderPass()
{
	VkAttachmentDescription attachment = {};
	attachment.format = VkContext::GetSwapChainImageFormat();
	attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	VkAttachmentReference color_attachment = {};
	color_attachment.attachment = 0;
	color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment;
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	VkRenderPassCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = 1;
	info.pAttachments = &attachment;
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	info.dependencyCount = 1;
	info.pDependencies = &dependency;
	if (vkCreateRenderPass(VkContext::GetDevice(), &info, nullptr, &m_ImguiRenderPass) != VK_SUCCESS)
	{
		throw std::exception("Failed Create Imgui render pass");
	}
}

void ImguiVulkan::CreateImguiCommandPool(VkCommandPool* commandPool)
{
	QueueFamilyIndices q = VkUtils::FindQueueFamilies(VkContext::GetPhysicalDevice());

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = q.graphicsFamily.value();

	if (vkCreateCommandPool(VkContext::GetDevice(), &poolInfo, nullptr, commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}


void ImguiVulkan::CreateImGuiCommandBuffers()
{
	m_ImGuiCommandBuffers.resize(VkContext::GetSwapChainImageViewsSize());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = m_CommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)m_ImGuiCommandBuffers.size();

	if (vkAllocateCommandBuffers(VkContext::GetDevice(), &allocInfo, m_ImGuiCommandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void ImguiVulkan::CreateImGuiFramebuffers()
{
		m_ImGuiFramebuffers.resize(VkContext::GetSwapChainImageViewsSize());

		auto swapChainExtent = VkContext::GetSwapChainVkExtent();

		VkImageView attachment[1];
		VkFramebufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.renderPass = m_ImguiRenderPass;
		info.attachmentCount = 1;
		info.pAttachments = attachment;
		info.width = swapChainExtent.width;
		info.height = swapChainExtent.height;
		info.layers = 1;
		for (uint32_t i = 0; i < VkContext::GetSwapChainImageViewsSize(); i++)
		{
			attachment[0] = VkContext::GetSwapChainImageViews()[i];
			if (vkCreateFramebuffer(VkContext::GetDevice(), &info, nullptr, &m_ImGuiFramebuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
}
