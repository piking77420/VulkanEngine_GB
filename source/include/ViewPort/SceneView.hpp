#pragma once
#include<Core/Core.h>
#include "Renderer/Vulkan/VulkanHeader.hpp"
#include "Core/ECS/System.hpp"

class SceneView : public System
{
public:

	virtual void Render(VulkanRendererData* datarenderer, Scene* scene);

	SceneView();
	~SceneView();

private:

};
