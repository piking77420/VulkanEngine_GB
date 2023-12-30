#pragma once
#include "Core/Core.h"
#include "IResource.hpp"


template<class T>
concept IResourceConcept = std::is_base_of<IResource, T>::value;

using ResourceTypeMap = std::map<std::string, IResource*>;


class ResourceManager;
static inline uint32_t RegisterResource();


template<class T>
class IRegisterResource : public IResource
{
public:

	virtual void LoadResource(const std::string& path) = 0 ;

	virtual void Destroy() override = 0;

	virtual void OnChange() override  = 0;
	
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
	static T* GetResource(const std::string& name)
	{
		//ISIDVALID(T::ID, m_ResourceMap);
		std::uint32_t id = T::ID;

		return reinterpret_cast<T*>(m_ResourceMap.at(id).at(name));
	}
	

	template<class T>
	static void Create(const std::string& path)
	{
		//static_assert(std::is_base_of<IRegisterResource, T>," is not a IRessource");

		std::string&& name = std::filesystem::path(path).stem().generic_string();
		IResource* newResource = new T();
		newResource->LoadResource(path);

		m_ResourceMap.at(T::ID).insert({ name,newResource});
	}

	template<class T>
	static void Create()
	{
		//static_assert(std::is_base_of<IRegisterResource, T>," is not a IRessource");

		IResource* newResource = new T();
		newResource->LoadResource("");

		std::string&& name = GetNameFromClassId(typeid(T).name() + ("_" + std::to_string(m_ResourceMap.at(T::ID).size())));

		m_ResourceMap.at(T::ID).insert({ name,newResource });
	}

	template<class T>
	static bool RemoveResource(const std::string& name)
	{
		ISIDVALID(T::ID, m_ResourceMap);

		if (!m_ResourceMap.at(T::ID).contains(name))
			return false;

		if (!m_ResourceMap.at(T::ID).contains(name))
			return false;

		m_ResourceMap.at(T::ID).erase(name);

		return true;
	}

	void DestroyAllResource();


	void GetVulkanRenderer()
	{
	}


	ResourceManager();

	

	static void AllocateRessouceManager()
	{
		if (S_RessourceManager)
			throw std::exception("try to init a second time a singleton");

		S_RessourceManager = new ResourceManager();
	}

	static void FreeSingleton()
	{
		delete S_RessourceManager;
	}

private:
	static inline std::map<uint32_t, ResourceTypeMap> m_ResourceMap;
	static inline ResourceManager* S_RessourceManager = nullptr;


	void LoadAllRessource();
	
	static std::string GetNameFromClassId(std::string&& rawName);
};

static inline uint32_t RegisterResource()
{
	return ResourceManager::GlobalMapResourceID++;
}

