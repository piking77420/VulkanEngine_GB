#include "Engine.hpp"
#include "Resource/Texture.hpp"
#include "Resource/Model.hpp"
#include "Renderer/System/RendereMesh.hpp"
#include "Renderer/MeshRenderer.h"
#include "ViewPort/Hierachy.hpp"
#include "ViewPort/SceneView.hpp"
#include "ViewPort/ContentBrowser.hpp"

void Engine::EngineRun()
{
	Camera* cam = Camera::SetMainCamera();

	

	
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwPollEvents();

		
		imguivulkan.NewFrame();

		scene.FixedUpdate();
		cam->UpdateMainCamera();
		scene.Update();
		m_VulkanRenderer->DrawScene(&scene);
		//imguivulkan.EndFrame();


	}
	vkDeviceWaitIdle(VkContext::GetDevice());

}

Engine::Engine()
{
	InitWindow();

	VkContext::CreateContext(m_Window);
	imguivulkan.InitImgui(m_Window);
	m_VulkanRenderer = new VulkanRenderer();

	scene.AddSystem<GraphScene>();
	scene.AddSystem<RendereMesh>();
	//scene.AddSystem<Hierachy>();
	scene.AddSystem<SceneView>();

	//scene.AddSystem<ContentBrowser>();


	scene.Begin();

}

Engine::~Engine()
{


	scene.~Scene();
	delete m_VulkanRenderer; 
	imguivulkan.DestroyImgui();
	VkContext::DestroyContext();
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
