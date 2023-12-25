#pragma once
#include<Imgui/imgui.h>
#include<Imgui/imgui_impl_glfw.h>
#include<Imgui/imgui_impl_vulkan.h>
#include <vulkan/vulkan.hpp>


class ImguiVulkan
{
public : 

	static inline bool   g_SwapChainRebuild = false;

	void InitImgui(GLFWwindow* mainWindow);

	void OnResizeFrammeBuffer();
		
	void ImguiNewFramme();

	void ImguiEndFramme();

	void DestroyImgui();

	~ImguiVulkan();

	bool main_is_minimized = false;

private:

	GLFWwindow* windowPtr;

	static void glfw_error_callback(int error, const char* description);
	
	static void check_vk_result(VkResult err);
	
	void SetupDescriptorPool();

	static bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
	
	static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

	void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
	void FramePresent(ImGui_ImplVulkanH_Window* wd);


	void BeginDocking();

	static inline VkAllocationCallbacks*   g_Allocator = nullptr;
	static inline VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
	static inline VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;
	static inline VkRenderPass g_RendererPass = VK_NULL_HANDLE;
	ImGui_ImplVulkanH_Window* wd;

	static inline ImGui_ImplVulkanH_Window g_MainWindowData;
	static inline int                      g_MinImageCount = 2;
};