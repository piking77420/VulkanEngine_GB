#pragma once

class Scene;
class VulkanRenderer;
class VulkanRendererData;

class System
{
public:

	virtual void Begin(Scene* scene) = 0 ;
	virtual void Update(Scene* scene) = 0 ;
	virtual void UpdateRender(VulkanRendererData* datarenderer, Scene* scene);
	virtual void Render(VulkanRendererData* datarenderer, Scene* scene) = 0 ;
	virtual void FixedUpdate(Scene* scene) = 0 ;


	virtual ~System() {};
};

