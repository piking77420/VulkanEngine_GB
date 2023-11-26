#pragma once
#include <Core/Core.h>
#include <GLFW/glfw3.h>
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
		if (enableValidationLayers) 
		{
			VkInit::DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
		}


		vkDestroyInstance(m_instance, nullptr);

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

private:
	
	const std::uint32_t m_Widht{ 800 };
	const std::uint32_t m_Height{ 600 };
	GLFWwindow* m_Window = nullptr;	
	VkInstance m_instance;
	VkDebugUtilsMessengerEXT m_debugMessenger;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device;
	VkQueue m_graphicsQueue;



    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
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
		PhysicalDevice::PickPhysicalDevice(m_instance, m_physicalDevice);
		Device::CreateLogicalDevice(m_device, m_physicalDevice, validationLayers, m_graphicsQueue);
	}

	




};

