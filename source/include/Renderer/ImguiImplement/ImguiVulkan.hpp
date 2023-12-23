#pragma once
#include <Renderer/Vulkan/VulkanConfig.hpp>
#include<Imgui/imgui.h>
#include<Imgui/imgui_impl_glfw.h>
#include<Imgui/imgui_impl_vulkan.h>



class ImguiVulkan
{
public : 

	static inline bool   g_SwapChainRebuild = false;

	void InitImgui(VulkanRendererData* vkdata,GLFWwindow* mainWindow);

	void OnResizeFrammeBuffer();

	void ImguiNewFramme();

	void ImguiEndFramme(VkCommandBuffer commandBuffer);

	void DestroyImgui();

	~ImguiVulkan();


private:
	void SetupVulkan(ImVector<const char*> instance_extensions);

	static void glfw_error_callback(int error, const char* description);
	
	static void check_vk_result(VkResult err);
	

	static bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
	
	static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);


	static VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
	

	static inline VkAllocationCallbacks*   g_Allocator = nullptr;
	static inline VkInstance               g_Instance = VK_NULL_HANDLE;
	static inline VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
	static inline VkDevice                 g_Device = VK_NULL_HANDLE;
	static inline uint32_t                 g_QueueFamily = (uint32_t)-1;
	static inline VkQueue                  g_Queue = VK_NULL_HANDLE;
	static inline VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
	static inline VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
	static inline VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;
	static inline VkRenderPass g_RendererPass = VK_NULL_HANDLE;


	static inline ImGui_ImplVulkanH_Window g_MainWindowData;
	static inline int                      g_MinImageCount = 2;
};