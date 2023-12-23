#include "ViewPort/SceneView.hpp"
#include "Renderer/ImguiImplement/ImguiVulkan.hpp"


void SceneView::Render(VulkanRendererData* datarenderer, Scene* scene)
{

	if(ImGui::Begin("SceneView"))
	{




		ImGui::End();
	}


}

SceneView::SceneView()
{
}

SceneView::~SceneView()
{
}