#pragma once
#include<Core/Core.h>
class Scene;
class VulkanRenderer;
class VulkanRendererData;

class System
{
public:

	virtual void Begin(Scene* scene) ;
	virtual void Update(Scene* scene) ;
	virtual void Render(VulkanRendererData* datarenderer, Scene* scene) ;
	virtual void FixedUpdate(Scene* scene) ;
	virtual void OnResizeData(uint32_t ComponentTypeID, std::vector<uint8_t>* data) {};


	virtual ~System() {};
};

