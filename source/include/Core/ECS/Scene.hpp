#pragma once
#include "Core/Core.h"
#include "Component.hpp"
#include "System.hpp"
#include "Physics/Transform.hpp"

using ComponentData = std::vector<uint8_t>;


class Scene
{
public : 

	Scene();
	
	
	~Scene()
	{
		
	}

	Entity& CreateEntity()
	{
		Entity ent = Entity(entity.size());
		entity.emplace_back(ent);
		AddComponent<Transform>(ent);
		return ent;
	}

	template<class T>
	inline static bool HasComponent(Entity entity)
	{
		return !(entity.ComponentId.at(T::ID) ? ComponentNull : 0);
	}

	template<class T>
	inline T* AddComponent(Entity& entity)
	{
		if (HasComponent<T>(entity))
			return nullptr;

		Component* ptr = nullptr;
		AddComponentInternal(entity, T::ID, &ptr);

		return reinterpret_cast<T*>(ptr);
	}

	template<class T>
	inline T* GetComponent(const Entity& entity)
	{
		if (!HasComponent<T>(entity))
			return nullptr;
		
		return reinterpret_cast<T*>(&componentData.at(T::ID).at(entity.ComponentId[T::ID]));
	}

	template<class T>
	inline const T* GetComponent(const Entity& entity) 
	{
		if (!HasComponent<T>(entity))
			return nullptr;

		return reinterpret_cast<const T*>(&componentData.at(T::ID).at(entity.ComponentId[T::ID]));
	}

	template<class T>
	void RemoveComponent(Entity& entity)
	{
		if (!HasComponent<T>(entity))
			return nullptr;

	}

	Entity* GetEntitiesById(const EntityID& entityId)
	{
		for (Entity& ent : entity)
		{
			if(ent.Id == entityId)
			{
				return &ent;
			}
		}
		return nullptr;
	}

	void RemoveEntiti(Entity& entity)
	{
		for(int i = 0 ; i < ComponentRegister::GetSizeComponentType();i++) 
		{
			RemoveComponentInternal(entity, i);
		}
	}

	template<class T>
	std::vector<T>* GetComponentData()
	{
		return reinterpret_cast<std::vector<T>*>(&componentData[T::ID]);
	}

	template<class T>
	void AddSystem()
	{
		systems.push_back(new T());
	}

	void Begin()
	{
		for (System* sys : systems)
		{
			sys->Begin();
		}
	}

	void Update()
	{
		for(System* sys : systems)
		{
			sys->Update(this);
		}
	}

	void FixedUpdate()
	{
		for (System* sys : systems)
		{
			sys->FixedUpdate();
		}
	}


private : 



	std::vector<ComponentData> componentData;

	std::vector<Entity> entity;

	std::vector<System*> systems;

	void SetComponentIdToEntity(Entity& entity, uint32_t ComponentID, uint32_t ComponentInMemoryindex);

	void AddComponentInternal(Entity& entity, uint32_t ComponentID, Component** ptr);

	void RemoveComponentInternal(Entity& entity, uint32_t ComponentID);
};

