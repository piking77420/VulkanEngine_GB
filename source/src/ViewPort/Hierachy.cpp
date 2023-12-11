#include "ViewPort/Hierachy.hpp"
#include "Physics/Transform.hpp"
#include "Core/ECS/Scene.hpp"
#include <Imgui/imgui.h>

void Hierachy::Begin(Scene* scene)
{
}

void Hierachy::Update(Scene* scene)
{
}


void Hierachy::Render(VulkanRendererData* datarenderer, Scene* scene)
{

	std::vector<Transform>* transform = scene->GetComponentData<Transform>();
	
	if(ImGui::Begin("Hierachy"))
	{
		

		for (std::vector<Transform>::iterator it = transform->begin(); it != transform->end(); it++)
		{
			Transform& ptr = *it;

			ImGui::CollapsingHeader(("entity " + std::to_string(ptr.entityId)).c_str());
			ImGui::PushID(ptr.entityId);

			ImGui::DragFloat3("Pos", ptr.pos.SetPtr());
			ImGui::DragFloat3("rotation", ptr.rotation.SetPtr());

			ImGui::DragFloat3("scale", ptr.scale.SetPtr());

			ImGui::PopID();


		}
		ImGui::End();
	}
}
