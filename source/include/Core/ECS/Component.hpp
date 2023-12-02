#pragma once
#include <Core/Core.h>


using EntityID = uint32_t;
using ComponentID = uint32_t;
constexpr EntityID EntityNull = std::numeric_limits<EntityID>::max();
constexpr uint32_t ComponentNull = std::numeric_limits<ComponentID>::max();
using EntityCompSubscrition = std::vector<ComponentID>;

struct Entity;




class Component
{
public :
	virtual ~Component(){}
	EntityID entityId = EntityNull;
};



using CompCreateFunc = std::function<void(std::vector<uint8_t>&,Entity&, Component**)>;
using FreeFunc = std::function<void(Component* ptr)>;


struct ComponentDataInfo
{
	CompCreateFunc CreecompFunc = nullptr;
	FreeFunc freeFunc = nullptr;
	size_t Size = -1;
};

class ComponentRegister
{
public:

	static inline uint32_t RegisterComponent(CompCreateFunc compFunc, FreeFunc freeFunc, size_t size)
	{
		uint32_t index = ComponentsInfo.size();
		ComponentDataInfo CdataInfo;
		
		CdataInfo.CreecompFunc = compFunc;
		CdataInfo.freeFunc = freeFunc;
		CdataInfo.Size = size;

		

		ComponentsInfo.insert(std::make_pair(index, CdataInfo));

		return index;
	}

	static __forceinline uint32_t GetSizeComponentType() noexcept
	{
		return ComponentsInfo.size();
	}


	static __forceinline CompCreateFunc GetCompCreateFunc(ComponentID id) noexcept
	{
		return ComponentsInfo.at(id).CreecompFunc;
	}

	static  __forceinline FreeFunc GetCompFreeFunc(ComponentID id) noexcept
	{
		return ComponentsInfo.at(id).freeFunc;
	}

	static  __forceinline size_t GetCompSize(ComponentID id) noexcept
	{
		return ComponentsInfo.at(id).Size;
	}

	

private:


	static inline std::map<uint32_t, ComponentDataInfo> ComponentsInfo;
};

struct Entity
{
	EntityID Id;
	EntityCompSubscrition ComponentId;

	Entity(std::uint32_t newId)
	{
		Id = newId;
		ComponentId.resize(ComponentRegister::GetSizeComponentType(), ComponentNull);
	}
};
