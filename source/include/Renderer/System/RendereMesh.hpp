#pragma once
#include "Core/ECS/System.hpp"
class Texture;

class RendereMesh : public System 
{

	void Begin(Scene* scene) override;

	void Update(Scene* scene) override;

	void Render(VulkanRendererData* datarenderer, Scene* scene) override;

	void FixedUpdate(Scene* scene) override;
private:
	void BindTexture(VulkanRendererData* datarenderer, Scene* scene, Texture* texture);
};

