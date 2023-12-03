#pragma once

class Scene;
class VulkanRenderer;

class System
{
public:

	virtual void Begin();
	virtual void Update(Scene* scene);
	virtual void Draw(VulkanRenderer* renderer);
	virtual void FixedUpdate();


	virtual ~System() {};
};

