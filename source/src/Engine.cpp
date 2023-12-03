#include "Engine.hpp"
#include "Resource/Texture.hpp"

Engine::Engine()
{
	InitWindow();
	m_VkRenderer.GetWindow(m_Window);

	m_VkRenderer.InitVulkan();

	m_ressourceManager.GetVulkanRenderer(m_VkRenderer);
	m_ressourceManager.Create<Texture>("Texture/statue.jpg");

	m_VkRenderer.GetRessourceManager(&m_ressourceManager);
	m_VkRenderer.InitRendering();




	scene.CreateEntity();
	scene.AddSystem<GraphScene>();
	scene.Begin();

}
