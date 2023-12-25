#include "ViewPort/SceneView.hpp"
#include "Renderer/ImguiImplement/ImguiVulkan.hpp"


void SceneView::Render(Scene* scene)
{
	
	ImGui::Begin("SceneView", &m_IsOpen);


	/*ImGui::Image((ImTextureID)0, ImGui::GetContentRegionAvail(),
		ImVec2(0, 1),
		ImVec2(1, 0));
		*/
	ImGui::End();	
}

SceneView::SceneView()
{
}

SceneView::~SceneView()
{
}