#pragma once
#include "Core/ECS/System.hpp"


class Hierachy :public System
{

	virtual void Begin(Scene* scene);
	virtual void Update(Scene* scene);
	virtual void Render(Scene* scene, VulkanRenderer* vkRenderer) override;

};

