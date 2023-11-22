#pragma once
#include "Renderer/Vulkan/VulkanRenderer.h"

class Engine
{
public:

	void EngineRun()
	{
		while (!glfwWindowShouldClose(m_Window)) {
			glfwPollEvents();
		}

	}
 
	void EngineInit()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_Window = glfwCreateWindow(m_Widht, m_Height, "Vulkan window", nullptr, nullptr);
		m_VulkanRenderer.InitVulkan(m_Window);

		
	}

	void DestroyEngine() 
	{
		glfwDestroyWindow(m_Window);
		m_VulkanRenderer.DestroyVulkan();
		glfwTerminate();
	}

private:
	
	const std::uint32_t m_Widht{ 800 };
	const std::uint32_t m_Height{ 600 };
	GLFWwindow* m_Window = nullptr;	
	VulkanRenderer m_VulkanRenderer;
};

