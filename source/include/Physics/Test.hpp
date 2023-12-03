#pragma once
#include "Core/ECS/IRegisterComponent.hpp"
#include "Transform.hpp"

class Test : public IRegisterComponent<Test>
{
public:
	Vector3 pos;
	Quaternion rotation;
	Vector3 scale;

	Test() {}
	~Test() {}
};