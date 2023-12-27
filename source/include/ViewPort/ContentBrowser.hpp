#pragma once
#include "Core/ECS/System.hpp"

class ContentBrowser : public System
{
public:
	virtual void Render(Scene* scene,VulkanRenderer* vkRenderer) override;
private:
	bool m_IsOpen = true;
};

