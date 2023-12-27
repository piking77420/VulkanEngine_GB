#include "ViewPort/ContentBrowser.hpp"
#include <Imgui/imgui.h>
#include "Renderer/Vulkan/VulkanRenderer.hpp"

void ContentBrowser::Render(Scene* scene, VulkanRenderer* vkRenderer)
{
	ImGui::Begin("ContentBrowser", &m_IsOpen);



	ImGui::End();

}
