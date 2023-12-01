#pragma once
#include "Component.hpp"


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

	static uint32_t ID;

private:

};


template<class T>
uint32_t IRegisterComponent<T>::ID = ComponentRegister::RegisterComponent(CreateComponent<T>, FreeComponent<T>, sizeof(T));
