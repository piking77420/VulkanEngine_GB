#pragma once
#include "Core/Core.h"
#include "Component.hpp"
#include "System.hpp"
#include "Physics/Transform.hpp"


class Engine;

class Scene
{
public : 

	Scene();
	
	~Scene()
	{
		for(System* sys: systems)
		{
			delete sys;
		}
	}

	Entity* CreateEntity()
	{
		Entity* ent = new Entity(entities.size());
		entities.emplace_back(ent);
		AddComponent<Transform>(ent);
		return ent;
	}

	template<class T>
	inline static bool HasComponent(Entity* entity)
	{
		if (entity->ComponentId.at(T::ID) == ComponentNull)
			return false;

		return true;
	}

	template<class T>
	inline T* AddComponent(Entity* entity)
	{

		static_assert(std::is_base_of<Component, T>::value, "T is not a subclass of Component");

		if (HasComponent<T>(entity))
			return nullptr;

		Component* ptr = nullptr;
		AddComponentInternal(*entity, T::ID, &ptr);

		return reinterpret_cast<T*>(ptr);
	}

	template<class T>
	inline T* GetComponent(Entity& entity)
	{
		static_assert(std::is_base_of<Component, T>::value, "T is not a subclass of Component");


		if (!HasComponent<T>(&entity))
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
	inline T* GetComponent(EntityID& entityID)
	{
		Entity& entity = *GetEntitiesById(entityID);

		if (!HasComponent<T>(&entity))
			return nullptr;


		return reinterpret_cast<T*>(&componentData.at(T::ID).at(entity.ComponentId[T::ID]));
	}

	template<class T>
	void RemoveComponent(Entity& entity)
	{
		if (!HasComponent<T>(entity))
			return nullptr;



	}

	Entity* GetEntitiesById(const EntityID& entityId)
	{
		for (Entity* ent : entities)
		{
			if(*ent == entityId)
			{
				return ent;
			}
		}
		return nullptr;
	}

	void RemoveEntitie(Entity& entity)
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
	std::vector<std::vector<T>*>* GetAllComponentFrom()
	{
		std::vector<std::vector<T>*>* dataOfAllComponent;

		

		return reinterpret_cast<std::vector<T>*>(&componentData[T::ID]);
	}


	template<class T>
	void AddSystem()
	{
		systems.push_back(new T());
	}


	template<class T>
	T* GetSystem()
	{
		for (size_t i = 0; i < systems.size(); i++)
		{
			T* tptr = dynamic_cast<T*>(systems[i]);
			if (tptr != nullptr)
			{
				return tptr;
			}
		}
		return nullptr;	
	}

	void Begin()
	{
		for (System* sys : systems)
		{
			sys->Begin(this);
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
			sys->FixedUpdate(this);
		}
	}

	
	void Render(VulkanRenderer* vkRenderer)
	{
		for (System* sys : systems)
		{
			sys->Render(this, vkRenderer);
		}
	}



private : 

	std::vector<ComponentData> componentData;

	std::vector<Entity*> entities;

	std::vector<System*> systems;

	void SetComponentIdToEntity(Entity& entity, uint32_t ComponentID, uint32_t ComponentInMemoryindex);

	void AddComponentInternal(Entity& entity, uint32_t ComponentID, Component** ptr);

	void RemoveComponentInternal(Entity& entity, uint32_t ComponentID);


	void TriggerOnresizeDataEvent(uint32_t componentTypeID, std::vector<uint8_t>* data)
	{
		for (size_t i = 0; i < systems.size(); i++)
		{
			systems[i]->OnResizeData(componentTypeID, data);
		}
	}

};

