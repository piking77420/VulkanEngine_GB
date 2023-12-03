#pragma once
#include "Component.hpp"

class Scene;

template<class T>
static inline void CreateComponent(std::vector<uint8_t>& compdata,Entity& entity, Component** ptr)
{
	uint32_t lastIndex = compdata.size();
	compdata.resize(lastIndex + sizeof(T));

	T* ptrToNew = new(&compdata[lastIndex])T();
	ptrToNew->entityId = entity.Id;
	*ptr = ptrToNew;
}

template<class T>
static inline void FreeComponent(Component* ptr)
{
	T* component = (T*)ptr;
	component->~T();
}



template<class T>
class IRegisterComponent : public Component
{
public:

	

	static const uint32_t ID;
private:
	friend Scene;
};


template<class T>
const uint32_t IRegisterComponent<T>::ID (ComponentRegister::RegisterComponent<T>(CreateComponent<T>, FreeComponent<T>, sizeof(T)));
