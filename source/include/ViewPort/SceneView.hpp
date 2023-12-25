#pragma once
#include<Core/Core.h>
#include "Core/ECS/System.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"


class SceneView : public System
{
public:

	virtual void Render(Scene* scene);

	SceneView();
	~SceneView();

private:
	bool m_IsOpen = true;
	VulkanRenderer m_VulkanRenderer;
	
};
