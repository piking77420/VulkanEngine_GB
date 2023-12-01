#include "Core/ECS/Scene.hpp"

Scene::Scene()
{
	componentData.resize(ComponentRegister::GetSizeComponentType());

}

void Scene::AddComponentInternal(Entity& entity, uint32_t ComponentID,Component** ptr)
{
	ComponentData& data = componentData.at(ComponentID);

	uint32_t index = data.size();

	auto creatfunc = ComponentRegister::GetCompCreateFunc(ComponentID);

	creatfunc(data,entity, ptr);
	SetComponentIdToEntity(entity, ComponentID, index);
}

void Scene::RemoveComponentInternal(Entity& entity, uint32_t ComponentID)
{
	if (ComponentID == Transform::ID)
		return;
	
	uint32_t index = entity.ComponentId.at(ComponentID);

	ComponentData& dataArray = componentData.at(ComponentID);
	FreeFunc freefn = ComponentRegister::GetCompFreeFunc(ComponentID);
	size_t sizeOfComponent = ComponentRegister::GetCompSize(ComponentID);

	//Last index for last Componenet
	uint32_t lastIndex = (uint32_t)(dataArray.size() - sizeOfComponent);

	Component* destComp = reinterpret_cast<Component*>(&dataArray[index]);
	Component* lastComp = reinterpret_cast<Component*>(&dataArray[lastIndex]);
	freefn(destComp);

	// the Component Free was the last so just resize it 
	if (index == lastIndex) {
		dataArray.resize(lastIndex);
		SetComponentIdToEntity(entity, ComponentID,ComponentNull);
		return;
	}


	Entity* entitylast = nullptr;
	entitylast = GetEntitiesById(lastComp->entityId);

	if (entitylast == nullptr)
		throw std::runtime_error("Entity not found for lastComp->entityID");


	memcpy(destComp, lastComp, sizeOfComponent);

	entitylast->ComponentId.at(ComponentID) = index;
	dataArray.resize(lastIndex);

	SetComponentIdToEntity(entity, ComponentID, ComponentNull);

}

void Scene::SetComponentIdToEntity(Entity& entity, uint32_t ComponentID,uint32_t ComponentInMemoryindex)
{
	entity.ComponentId[ComponentID] = ComponentInMemoryindex;
}

