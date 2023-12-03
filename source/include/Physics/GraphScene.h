#pragma once
#include "Core/ECS/System.hpp"

class GraphScene : public System
{
public:
	virtual void Begin(Scene* scene) override ;
	virtual void Update(Scene* scene) override ;
	virtual void FixedUpdate(Scene* scene) override ;
	virtual void Render(VulkanRendererData* datarenderer, Scene* scene) override;

};

