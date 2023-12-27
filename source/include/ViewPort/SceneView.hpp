#pragma once
#include<Core/Core.h>
#include "Core/ECS/System.hpp"
#include "Renderer/Vulkan/VulkanRenderer.hpp"


class SceneView : public System
{
public:

	virtual void Begin(Scene* scene) override;

	SceneView();
	~SceneView();

private:
	bool m_IsOpen = true;
};
