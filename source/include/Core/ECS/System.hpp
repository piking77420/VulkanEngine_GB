#pragma once
#include<Core/Core.h>
class Scene;

class System
{
public:

	virtual void Begin(Scene* scene) ;
	virtual void Update(Scene* scene) ;
	virtual void Render(Scene* scene) ;
	virtual void FixedUpdate(Scene* scene) ;
	virtual void OnResizeData(uint32_t ComponentTypeID, std::vector<uint8_t>* data) {};


	virtual ~System() {};
};

