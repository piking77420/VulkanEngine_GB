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
		imguivulkan.ImguiNewFramme();
		scene.FixedUpdate();
		cam->UpdateMainCamera();
		scene.Update();
		scene.Render();
		imguivulkan.ImguiEndFramme();

	}

}

Engine::Engine()
{
	InitWindow();

	VkContext::CreateContext(m_Window);
	imguivulkan.InitImgui(m_Window);

	scene.AddSystem<GraphScene>();
	scene.AddSystem<RendereMesh>();
	scene.AddSystem<Hierachy>();
	scene.AddSystem<SceneView>();
	scene.AddSystem<ContentBrowser>();


	scene.Begin();

}

Engine::~Engine()
{

	VkResult err = vkDeviceWaitIdle(VkContext::GetDevice());
	CheckVkResult(err, "Failed DeviceWaitIdle on destroy Engin");	


	imguivulkan.DestroyImgui();
	VkContext::DestroyContext();
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
