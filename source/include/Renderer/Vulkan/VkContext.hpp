#pragma once
#include "vulkan/vulkan.hpp" 
#include "VulkanConfig.h"
#include "Core/Core.h"

struct GLFWwindow;
class VkDepth;


static inline void CheckVkResult(VkResult result,const char* messageOnFail)
{
	if (result != VK_SUCCESS)
		throw std::exception(messageOnFail);
}

class VulkanRenderer;

class VkContext
{
public: 

	static const VkInstance& GetInstance()
	{
		return m_Instance;
	}


	static const VkDevice& GetDevice()
	{
		return m_Device;
	}


	static const VkPhysicalDevice& GetPhysicalDevice()
	{
		return m_PhysicalDevice;
	}


	static const VkQueue& GetGraphicQueue()
	{
		return m_GraphicsQueue;
	}


	static const VkQueue& GetPresentQueue()
	{
		return m_PresentQueue;
	}
	static const VkSurfaceKHR& GetSurface()
	{
		return m_Surface;
	}


	static const std::vector<const char*>& GetDeviceExtensions()
	{
		return m_DeviceExtensions;
	}

	static VkSwapchainKHR& GetSwapChain()
	{
		return m_SwapChain;
	}


	static VkCommandPool& GetCommandPool()
	{
		return m_CommandPool;
	}
	

	static void CreateContext(const GLFWwindow const* window);
	static void DestroyContext();





	static std::uint32_t GetSwapChainImageViewsSize()
	{
		return m_SwapChainImageViews.size();
	}

	static std::vector<VkImageView>& GetSwapChainImageViews()
	{
		return m_SwapChainImageViews;
	}

	static VkFormat GetSwapChainImageFormat()
	{
		return m_SwapChainImageFormat;
	}


	static VkExtent2D GetSwapChainVkExtent()
	{
		return m_SwapChainExtent;
	}



	static std::vector<VkSemaphore>& GetImageAvailableSemaphore()
	{
		return m_ImageAvailableSemaphores;
	}

	static std::vector<VkSemaphore>& GetRenderFinishedSemaphore()
	{
		return m_RenderFinishedSemaphores;
	}
	static std::vector<VkFence>& GetInFlightFence()
	{
		return m_InFlightFences;
	}

	static VkRenderPass&  GetRenderPass()
	{
		return m_RenderPass;
	}

	static std::vector<VkFramebuffer>& GetSwapChainFramebuffers()
	{
		return m_SwapChainFramebuffers;
	}


	static uint32_t& GetCurrentFramme()
	{
		return m_CurrentFrame;
	}

	static VkSampler GetGetTextureSampler()
	{
		return m_TextureSampler;
	}

	static void NextFrame();
	
	VkContext() = delete;
	~VkContext() = delete;

	static void RecreateSwapChain();
	static inline bool FramebufferResized = false;

	static inline std::uint32_t ImageIndex = 0;
private:
	static VkInstance m_Instance ;
	static VkDevice m_Device ;
	static VkPhysicalDevice m_PhysicalDevice ;
	static inline VkDebugUtilsMessengerEXT m_DebugMessenger;
	static inline VkQueue m_GraphicsQueue;
	
	static inline VkSurfaceKHR m_Surface;
	static inline VkQueue m_PresentQueue;
	


	static inline VkSwapchainKHR m_SwapChain;

	static inline std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	static inline std::vector<VkImage> m_SwapChainImages;
	static inline std::vector<VkImageView> m_SwapChainImageViews;

	static inline VkFormat m_SwapChainImageFormat;
	static inline VkExtent2D m_SwapChainExtent;

	static inline VkCommandPool m_CommandPool;
	static inline VkRenderPass m_RenderPass;
	static VkDepth vkDepth;


	static inline std::vector<VkSemaphore> m_ImageAvailableSemaphores;
	static inline std::vector<VkSemaphore> m_RenderFinishedSemaphores;
	static inline std::vector<VkFence> m_InFlightFences;



	static inline std::vector<const char*> layers;

	static inline uint32_t m_CurrentFrame = 0;

	static inline VkSampler m_TextureSampler;


	// viewport
	/*
	static inline std::vector<VkImage> m_ViewportImages;
	static inline std::vector<VkDeviceMemory> m_DstImageMemory;
	static inline std::vector<VkImageView> m_ViewportImageViews;
	static inline VkRenderPass m_ViewportRenderPass;
	static inline VkPipeline m_ViewportPipeline;
	static inline VkCommandPool m_ViewportCommandPool;
	static inline std::vector<VkFramebuffer> m_ViewportFramebuffers;
	static inline std::vector<VkCommandBuffer> m_ViewportCommandBuffers;
	*/




	static inline const std::vector<const char*> m_DeviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};



	static void CreateInstance();
	static void InitPhysicalDevice();
	static void InitDevice();
	static bool EnumerateSupportedExtension(std::vector<const char* >& extensions, std::vector<const char* >& layers);
	static void DebugMessenger();

	static void CreateSwapChain();
	static void CreateFramebuffers();
	static void CreateImageViews();

	static void CreateRenderPass();


	static void CreateSyncObjects();

	static void CleanupSwapChain();
	static void CreateCommandPool(VkCommandPool& commandPool);
	static void CreateTextureSampler();
	

	// viewPort //
	static void CreateViewportImage();
	static void CreateViewportImageViews();
	static void CreateViewportFrameBuffers();



	static inline std::vector<VkDynamicState> m_DynamicStates =
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};



};

