#include "Resource/ResourceManager.hpp"

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
}
