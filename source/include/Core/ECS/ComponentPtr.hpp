#pragma once
#include "Core/Core.h"
#include "Component.hpp"

template<class T>
class ComponentPtr
{
public:
	ComponentPtr(){};
	~ComponentPtr(){};

	void MakePtr(T* _ptr) { ptr = _ptr; }

	ComponentPtr* operator->() { return ptr; }
	ComponentID IdComponent;

private:
	ComponentPtr* ptr;
};
