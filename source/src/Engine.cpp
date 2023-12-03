#include "Engine.hpp"
#include "Resource/Texture.hpp"

Engine::Engine()
{
	InitWindow();
	m_VkRenderer.GetWindow(m_Window);
	m_VkRenderer.InitVulkan();
	m_ressourceManager.GetVulkanRenderer(m_VkRenderer);
	m_ressourceManager.Create<Texture>("Texture/statue.jpg");
	scene.CreateEntity();
	scene.AddSystem<GraphScene>();
	scene.Begin();
}
