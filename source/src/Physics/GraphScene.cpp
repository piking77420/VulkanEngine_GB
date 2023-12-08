#include "Physics/GraphScene.h"
#include "Core/ECS/Scene.hpp"
#include "Renderer/Vulkan/VulkanConfig.hpp"

void GraphScene::Begin(Scene* scene)
{

}

void GraphScene::Update(Scene* scene)
{
	std::vector<Transform>* data = scene->GetComponentData<Transform>();


	for (std::vector<Transform>::iterator it = data->begin(); it != data->end(); it++)
	{
		it->GetQuatrion() = Quaternion::FromEulerAngle(it->rotation);
		it->Local = Matrix4X4::TRS(it->pos, it->GetQuatrion(), it->scale);
		it->Global = it->Local;
	}

}


void GraphScene::FixedUpdate(Scene* scene)
{

}

void GraphScene::Render(VulkanRendererData* datarenderer, Scene* scene)
{
}
