#pragma once
#define GLFW_INCLUDE_VULKAN
#include <Core/Core.h>
#include "Renderer/Vulkan/VulkanRenderer.hpp"




class Engine
{
public:

	void EngineRun()
	{
		while (!glfwWindowShouldClose(m_Window)) {
			glfwPollEvents();
			m_VkRenderer.Draw();

		}
		m_VkRenderer.RendererWait();
	}
 

	Engine()
	{
		InitWindow();
		m_VkRenderer.InitVulkan(m_Window);
	}

	~Engine()
	{
		//CleanUpVulkan();
		m_VkRenderer.CleanUpVulkan();
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

private:
	
	const std::uint32_t m_Widht{ 800 };
	const std::uint32_t m_Height{ 600 };
	GLFWwindow* m_Window = nullptr;	
	VulkanRenderer m_VkRenderer;

	void InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_Window = glfwCreateWindow(m_Widht, m_Height, "Vulkan window", nullptr, nullptr);
	}

	
};

