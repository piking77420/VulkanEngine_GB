#pragma once
#define GLFW_INCLUDE_VULKAN
#include <Core/Core.h>
#include "Renderer/Vulkan/VulkanHeader.hpp"

#ifdef NDEBUG
const static inline bool enableValidationLayers = false;
#else
const static inline bool enableValidationLayers = true;
#endif


class Engine
{
public:

	void EngineRun()
	{
		while (!glfwWindowShouldClose(m_Window)) {
			glfwPollEvents();
		}
			
	}
 

	Engine()
	{
		InitWindow();
		InitVulkan();
	}

	~Engine()
	{
		CleanUpVulkan();
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

private:
	
	const std::uint32_t m_Widht{ 800 };
	const std::uint32_t m_Height{ 600 };
	GLFWwindow* m_Window = nullptr;	
	VkInstance m_instance;
	VkDebugUtilsMessengerEXT m_debugMessenger;
	VkSurfaceKHR m_surface;


	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	VkSwapchainKHR m_swapChain;
	std::vector<VkImage> m_swapChainImages;
	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

	std::vector<VkImageView> m_swapChainImageViews;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	void InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_Window = glfwCreateWindow(m_Widht, m_Height, "Vulkan window", nullptr, nullptr);
	}

	void InitVulkan()
	{
		VulkanInstance::CreateInstance(&m_instance,validationLayers);
		VkInit::SetupDebugMessenger(m_instance,&m_debugMessenger);
		VkInit::CreateSurface(&m_instance,m_Window,&m_surface);
		VkInit::PickPhysicalDevice(m_instance, m_physicalDevice, m_surface);
		Device::CreateLogicalDevice(m_device, m_physicalDevice, validationLayers, m_graphicsQueue,m_surface,deviceExtensions);
		VkUtils::CreateSwapChain(m_Window,m_physicalDevice,m_surface,m_swapChain,m_device,m_swapChainImages,m_swapChainImageFormat,m_swapChainExtent);
		VkUtils::CreateImageViews(m_swapChainImages, m_swapChainImageFormat, m_swapChainImageViews, m_device);
	}

	void CleanUpVulkan()
	{
		for (auto imageView : m_swapChainImageViews) {
			vkDestroyImageView(m_device, imageView, nullptr);
		}


		vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
		vkDestroyDevice(m_device, nullptr);

		if (enableValidationLayers) {
			VkInit::DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyInstance(m_instance, nullptr);
	}





};

