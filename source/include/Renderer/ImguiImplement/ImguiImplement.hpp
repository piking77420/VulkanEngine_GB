#include <Renderer/Vulkan/VulkanConfig.hpp>
#include<Imgui/imgui.h>
#include<Imgui/imgui_impl_glfw.h>
#include<Imgui/imgui_impl_vulkan.h>



class ImguiVulkan
{
public : 

	
	void InitImguiPipline();
	void GetVulkanData(class VulkanRendererData* vkdata);

	~ImguiVulkan();


private:
	VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
	void CreateDevice();


	static inline VkPipeline ImguiPipeLine = VK_NULL_HANDLE;


	// Data
	static inline VkAllocationCallbacks*	g_Allocator = nullptr;
	static inline VkInstance               g_Instance = VK_NULL_HANDLE;
	static inline VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
	static inline VkDevice                 g_Device = VK_NULL_HANDLE;
	static inline uint32_t                 g_QueueFamily = (uint32_t)-1;
	static inline VkQueue                  g_Queue = VK_NULL_HANDLE;
	static inline VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
	static inline VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
	static inline VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

	static inline ImGui_ImplVulkanH_Window g_MainWindowData;
	static inline int                      g_MinImageCount = 2;
	static inline bool                     g_SwapChainRebuild = false;

};