#pragma once
#include "Core/ECS/System.hpp"

class RendereMesh : public System 
{

	void Begin(Scene* scene) override;

	void Update(Scene* scene) override;

	void UpdateRender(VulkanRendererData* datarenderer, Scene* scene) override;


	void Render(VulkanRendererData* datarenderer, Scene* scene) override;

	void FixedUpdate(Scene* scene) override;
};

