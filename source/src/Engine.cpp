#include "Engine.hpp"
#include "Resource/Texture.hpp"
#include "Resource/Model.hpp"
#include "Renderer/System/RendereMesh.hpp"
#include "Renderer/MeshRenderer.h"
#include "ViewPort/Hierachy.hpp"
#include "ViewPort/SceneView.hpp"
#include "ViewPort/ContentBrowser.hpp"
#include "Resource/Material.hpp"

void Engine::EngineRun()
{
	Camera* cam = Camera::SetMainCamera();
	
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwPollEvents();

		
		imguivulkan.NewFrame();

		Camera::GetMainCamera()->UpdateMainCamera();
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
	ResourceManager::AllocateRessouceManager();
	m_VulkanRenderer = new VulkanRenderer();

	scene.AddSystem<GraphScene>();
	//scene.AddSystem<Hierachy>();
	scene.AddSystem<RendereMesh>();
	//scene.AddSystem<SceneView>();

	Material* material = ResourceManager::GetResource<Material>("Material_0");
	material->baseColor = ResourceManager::GetResource<Texture>("viking_room");
	material->OnChange();

	Entity* ent =  scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(ent)->model = ResourceManager::GetResource<Model>("viking_room");
	scene.GetComponent<MeshRenderer>(*ent)->material = ResourceManager::GetResource<Material>("Material_0");

	Entity* ent2 = scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(ent2)->model = ResourceManager::GetResource<Model>("viking_room");
	scene.GetComponent<MeshRenderer>(*ent2)->material = ResourceManager::GetResource<Material>("Material_0");

	

	//scene.AddSystem<ContentBrowser>();

	

	scene.Begin();

}

Engine::~Engine()
{


	scene.~Scene();
	ResourceManager::FreeSingleton();
	delete m_VulkanRenderer; 
	imguivulkan.DestroyImgui();
	VkContext::DestroyContext();
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
