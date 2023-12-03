#pragma once
#include "Core/Core.h"
#include "IResource.hpp"

template<class T>
concept IResourceConcept = std::is_base_of<IResource, T>::value;

using IResourcePtr = std::unique_ptr<IResource>;
using ResourceTypeMap = std::map<std::string, IResourcePtr>;


static inline uint32_t RegisterResource();


template<class T>
class IRegisterResource : IResource
{
	static inline uint32_t ID = RegisterResource();
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
	void Create(const std::string& name)
	{
		static_assert(std::is_base_of<IResource, T>," is not a IRessource");

		IResourcePtr newResource = std::make_unique<IResourcePtr>(new T());

		m_ResourceMap.at(T::ID).insert({ name ,newResource });
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


private:
	std::map<uint32_t, ResourceTypeMap> m_ResourceMap;
};

static inline uint32_t RegisterResource()
{
	return ResourceManager::GlobalMapResourceID++;
}

