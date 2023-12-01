#pragma once
#define GLFW_INCLUDE_VULKAN
#include <Core/Core.h>
#include "Renderer/Vulkan/VulkanRenderer.hpp"
#include "Core/ECS/Scene.hpp"
#include "Physics/GraphScene.h"


class Engine
{
public:

	void EngineRun()
	{

		while (!glfwWindowShouldClose(m_Window)) {
			glfwPollEvents();
			scene.FixedUpdate();
			scene.Update();
			m_VkRenderer.Draw();
		}
		m_VkRenderer.RendererWait();
	}
 

	Engine()
	{
		InitWindow();
		m_VkRenderer.GetWindow(m_Window);
		m_VkRenderer.InitVulkan();
		scene.CreateEntity();
		scene.AddSystem<GraphScene>();
		scene.Begin();
	}

	~Engine()
	{
		
		m_VkRenderer.CleanUpVulkan();
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}



private:
	
	const std::uint32_t m_Widht{ 800 };
	const std::uint32_t m_Height{ 600 };
	GLFWwindow* m_Window = nullptr;	
	VulkanRenderer m_VkRenderer;
	Scene scene;

	void InitWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(m_Widht, m_Height, "Vulkan window", nullptr, nullptr);
		// &m_VkRenderer is correct dont move it 
		glfwSetWindowUserPointer(m_Window, &m_VkRenderer);
		glfwSetFramebufferSizeCallback(m_Window, FramebufferResizeCallback);

	}


	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto vulkanRenderer = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
		vulkanRenderer->framebufferResized = true;
	}
};

