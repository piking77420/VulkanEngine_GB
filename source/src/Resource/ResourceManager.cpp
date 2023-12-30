#include "Resource/ResourceManager.hpp"
#include "Resource/Texture.hpp"
#include "Resource/Model.hpp"
#include "Resource/Material.hpp"

void ResourceManager::DestroyAllResource()
{
	for (auto it = m_ResourceMap.begin() ; it != m_ResourceMap.end() ; it++)
	{
		for (auto at = it->second.begin(); at != it->second.end(); at++)
		{
			at->second->Destroy();
			delete at->second;
		}
	}
}

ResourceManager::ResourceManager()
{


	for (size_t i = 0; i < GlobalMapResourceID; i++)
	{
		m_ResourceMap.insert({ i,std::map<std::string,IResource*>() });
	}

	LoadAllRessource();
}

void ResourceManager::LoadAllRessource()
{

	// Texture
	Create<Texture>("Texture/viking_room.png");
	Create<Texture>("Texture/statue.jpg");

	// Model
	Create<Model>("Model/viking_room.obj");
	Create<Model>("Model/chinesedragon.gltf");


	Create<Material>();



}

std::string ResourceManager::GetNameFromClassId(std::string&& rawName)
{
	std::string name;
	name.resize(rawName.size() - 6);

	for (size_t i = 0; i < rawName.size() - 6; i++)
	{
		name[i] = rawName[i + 6];
	}


	return name;
}
