#pragma once

class Scene;

class System
{
public:

	virtual void Begin();
	virtual void Update(Scene* scene);
	virtual void FixedUpdate();


	virtual ~System() {};
};

