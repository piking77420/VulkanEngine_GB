#pragma once
#include "Core/ECS/System.hpp"

class ContentBrowser : public System
{
public:
	virtual void Render(Scene* scene) override;
private:
	bool m_IsOpen = true;
};

