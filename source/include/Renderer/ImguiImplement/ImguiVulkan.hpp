#pragma once
#include<Imgui/imgui.h>
#include<Imgui/imgui_impl_glfw.h>
#include<Imgui/imgui_impl_vulkan.h>
#include <vulkan/vulkan.hpp>
#include "Renderer/Vulkan/VulkanRenderer.hpp"

class ImguiVulkan 
{
public : 

	
	void InitImgui(GLFWwindow* window);
	void DestroyImgui();
	void NewFrame();
	void EndFrame();
	void ImguiRender();

	void OnResizeFrammebuffer();

	std::vector<VkCommandBuffer> m_ImGuiCommandBuffers;
	std::vector<VkFramebuffer> m_ImGuiFramebuffers;
	static inline std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> m_Dset;

private:

	void InitImgui();
	void CreateImGuiRenderPass();
	void CreateImguiCommandPool(VkCommandPool* commandPool);
	void CreateImGuiCommandBuffers();
	void CreateImGuiFramebuffers();

	void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);




	VkRenderPass m_ImguiRenderPass;
	VkDescriptorPool m_ImGuiDescriptorPool;
	VkCommandPool m_CommandPool;

	std::uint32_t m_ImageCount = 2;
};