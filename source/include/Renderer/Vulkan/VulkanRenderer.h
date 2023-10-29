#pragma once
#include "Renderer/Vulkan/VulkanInit.hpp"
#include "Renderer/Vulkan/VulkanSwapChain.hpp"


class VulkanRenderer
{
public:


	void InitVulkan(GLFWwindow* _Window)
	{
		m_Instance = vkInit::MakeInstance(m_Debug, "Vulkan window");
		m_dldi = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);
		if (m_Debug)
		{
			m_DebugMessengers = vkInit::MakeDebugLogger(m_Instance, m_dldi);
		}
		LinkSurfaceVulkanGLFW(_Window);
		InitVulkanDevice();
	}

	void EnumeRateProperties()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::cout << extensionCount << " extensions supported\n";
	}

	void DestroyVulkan()
	{
		for (vkUtil::SwapChainFrame frame : m_SwapChainFrames)
		{
			m_Device.destroyImageView(frame.imageView);
		}

		m_Device.destroySwapchainKHR(m_SwapChain);
		m_Device.destroy();
		m_Instance.destroySurfaceKHR(m_Surface);
		m_Instance.destroy();

		std::cout << " End Of Engine \n";

	}



	void InitVulkanDevice()
	{
		m_PhysicalDevice = vkInit::GetPhyscialDevice(m_Instance, m_Debug);
		m_Device = vkInit::CreateLogicalDevice(m_PhysicalDevice, m_Surface, m_Debug);
		std::array<vk::Queue, 2> vkQueueArray = vkInit::GetQueue(m_PhysicalDevice, m_Device, m_Surface, m_Debug);
		m_GraphicQueue = vkQueueArray[0];
		m_PresentQueue = vkQueueArray[1];
		vkInit::SwapChainBundle swapChainBundle = vkInit::CreateSwapchain(m_Device, m_PhysicalDevice, m_Surface, m_Widht, m_Height, m_Debug);

		m_SwapChain = swapChainBundle.swapchain;
		m_SwapChainFrames = swapChainBundle.frames;

		m_FormatSwapChain = swapChainBundle.format;
		m_ExtentSwapChain = swapChainBundle.extent;
	}

private:

	void LinkSurfaceVulkanGLFW(GLFWwindow* _Window)
	{
		VkSurfaceKHR c_style_surfce;
		if (glfwCreateWindowSurface(m_Instance, _Window, nullptr, &c_style_surfce) != VK_SUCCESS)
		{
			if (m_Debug)
			{
				std::cout << " Failed to abstact the glfw surface for vulkan. \n";

			}
		}
		if (m_Debug)
		{
			std::cout << " Succesfully to abstact the glfw surface for vulkan. \n";

		}
		m_Surface = c_style_surfce;
	}


	bool m_Debug = true;
	const std::uint32_t m_Widht{ 800 };
	const std::uint32_t m_Height{ 600 };

	// Instance
	vk::Instance m_Instance = nullptr;
	vk::DebugUtilsMessengerEXT m_DebugMessengers = nullptr;
	vk::DispatchLoaderDynamic m_dldi;
	vk::SurfaceKHR m_Surface{ nullptr };

	// Device
	vk::PhysicalDevice m_PhysicalDevice{ nullptr };
	vk::Device m_Device{ nullptr };
	// Queue
	vk::Queue m_GraphicQueue{ nullptr };
	vk::Queue m_PresentQueue{ nullptr };

	// SwapChain
	vk::SwapchainKHR m_SwapChain{ nullptr };
	std::vector<vkUtil::SwapChainFrame> m_SwapChainFrames;
	vk::Format m_FormatSwapChain;
	vk::Extent2D m_ExtentSwapChain;

	


};

