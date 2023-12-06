#include "Engine.hpp"
#include "Resource/Texture.hpp"
#include "Resource/Model.hpp"
#include "Renderer/System/RendereMesh.hpp"
#include "Renderer/MeshRenderer.h"



Engine::Engine()
{
	InitWindow();
	m_VkRenderer.GetWindow(m_Window);

	m_VkRenderer.InitVulkan();

	m_ressourceManager.GetVulkanRenderer(m_VkRenderer);
	m_ressourceManager.Create<Texture>("Texture/viking_room.png");
	m_VkRenderer.GetRessourceManager(&m_ressourceManager);
	m_VkRenderer.InitRendering();



	m_ressourceManager.Create<Model>("Model/viking_room.obj");
	// Init Scene // 


	Entity* ent =  scene.CreateEntity();
	
	scene.AddComponent<MeshRenderer>(ent);
	MeshRenderer* mesh  = scene.GetComponent<MeshRenderer>(*ent);
	mesh->model = m_ressourceManager.GetResource<Model>("Model/viking_room.obj");

	scene.AddSystem<GraphScene>();
	scene.AddSystem<RendereMesh>();
	scene.Begin();

}
