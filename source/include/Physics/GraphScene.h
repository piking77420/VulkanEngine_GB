#pragma once
#include "Core/ECS/System.hpp"

class GraphScene : public System
{

	virtual void Begin() override ;
	virtual void Update(Scene* scene) override ;
	virtual void FixedUpdate() override ;

};

