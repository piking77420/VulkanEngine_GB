#pragma once
#include "Core/Core.h"
#include "IResource.hpp"

class VulkanRendererData;

template<class T>
concept IResourceConcept = std::is_base_of<IResource, T>::value;

using IResourcePtr = IResource*;
using ResourceTypeMap = std::map<std::string, IResourcePtr>;


class ResourceManager;
static inline uint32_t RegisterResource();


template<class T>
class IRegisterResource : IResource
{
public:


	void Destroy(VulkanRendererData& data) override
	{
	}
protected :
	static inline uint32_t ID = RegisterResource();
	friend ResourceManager;
};

#define ISIDVALID(ID,ResourceMap) assert(ID <= ResourceMap.size()); \
\
static_assert(std::is_base_of<IResource, T>," is not a IRessource");\



class ResourceManager
{
public:
	static inline uint32_t GlobalMapResourceID = 0;

	template<class T>
	T* GetResource(const std::string& name)
	{
		ISIDVALID(T::ID, m_ResourceMap);

		m_ResourceMap.at(T::ID).at(name);
	}
	template<class T>
	const T* GetResource(const std::string& name) const 
	{
		ISIDVALID(T::ID, m_ResourceMap);

		m_ResourceMap.at(T::ID).at(name);
	}

	template<class T>
	void Create(const std::string& path)
	{
		//static_assert(std::is_base_of<IRegisterResource, T>," is not a IRessource");

		IResource* newResource = new T();
		newResource->LoadResource(path,*m_VulkanRender);


		m_ResourceMap.at(T::ID).insert({ path ,newResource });
	}

	template<class T>
	bool RemoveResource(const std::string& name) const
	{
		ISIDVALID(T::ID, m_ResourceMap);

		if (!m_ResourceMap.at(T::ID).contains(name))
			return false;

		if (!m_ResourceMap.at(T::ID).contains(name))
			return false;

		m_ResourceMap.at(T::ID).erase(name);

		return true;
	}

	void DestroyAllResource(VulkanRendererData& _VulkanRendererData);


	void GetVulkanRenderer(VulkanRendererData& _vulkanRendererData)
	{
		m_VulkanRender = &_vulkanRendererData;
	}


	ResourceManager();

private:
	std::map<uint32_t, ResourceTypeMap> m_ResourceMap;
	VulkanRendererData* m_VulkanRender;

};

static inline uint32_t RegisterResource()
{
	return ResourceManager::GlobalMapResourceID++;
}

