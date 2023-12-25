#pragma once
#define GLFW_INCLUDE_VULKAN
#include <Core/Core.h>
#include "Core/ECS/Scene.hpp"
#include "Physics/GraphScene.h"

#include "Renderer/ImguiImplement/ImguiVulkan.hpp"
#include "Resource/ResourceManager.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer\ImguiImplement\ImguiVulkan.hpp"
#include "Renderer/Vulkan/VkContext.hpp"

class Engine
{
public:

	void EngineRun();
	
 

	Engine();
	

	~Engine();
	
	static GLFWwindow* GetWindow()
	{
		return m_Window;
	}


private:
	
	const std::uint32_t m_Widht{ 1920 };
	const std::uint32_t m_Height{ 1080 };
	static inline GLFWwindow* m_Window = nullptr;	
	Scene scene;
	ResourceManager m_ressourceManager;
	ImguiVulkan imguivulkan;



	void InitWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(m_Widht, m_Height, "Vulkan window", nullptr, nullptr);


		glfwSetCursorPosCallback(m_Window, Camera::MouseCallback);

		glfwSetFramebufferSizeCallback(m_Window, FramebufferResizeCallback);


		 glfwMakeContextCurrent(m_Window);

		static GLFWwindow* currentContext = glfwGetCurrentContext();
	}


	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto vulkanRenderer = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
		vulkanRenderer->imguivulkan.g_SwapChainRebuild == true;

	}
};

