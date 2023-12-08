#pragma once

class Scene;
class VulkanRenderer;
class VulkanRendererData;

class System
{
public:

	virtual void Begin(Scene* scene) ;
	virtual void Update(Scene* scene) ;
	virtual void UpdateRender(VulkanRendererData* datarenderer, Scene* scene);
	virtual void Render(VulkanRendererData* datarenderer, Scene* scene) ;
	virtual void FixedUpdate(Scene* scene) ;


	virtual ~System() {};
};

