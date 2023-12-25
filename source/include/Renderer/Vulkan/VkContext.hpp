#pragma once
#include "vulkan/vulkan.hpp" 
#include "Core/Core.h"

struct GLFWwindow;

static inline void CheckVkResult(VkResult result,const char* messageOnFail)
{
	if (result != VK_SUCCESS)
		throw std::exception(messageOnFail);
}


class VkContext
{
public: 

	static const VkInstance& GetInstance()
	{
		return m_Instance;
	}


	static const VkDevice& GetDevice()
	{
		return m_Device;
	}


	static const VkPhysicalDevice& GetPhysicalDevice()
	{
		return m_PhysicalDevice;
	}


	static const VkQueue& GetGraphicQueue()
	{
		return m_GraphicsQueue;
	}

	static const VkSurfaceKHR& GetSurface()
	{
		return m_Surface;
	}

	static const VkQueue& GetPresentQueue()
	{
		return m_PresentQueue;
	}


	static void CreateContext(const GLFWwindow const* window);
	static void DestroyContext();



	VkContext() = delete;
	~VkContext() = delete;



private:
	static VkInstance m_Instance ;
	static VkDevice m_Device ;
	static VkPhysicalDevice m_PhysicalDevice ;
	static inline VkDebugUtilsMessengerEXT m_DebugMessenger;
	static inline VkQueue m_GraphicsQueue;
	
	static inline VkSurfaceKHR m_Surface;
	static inline VkQueue m_PresentQueue;

	static inline std::vector<const char*> layers;


	static void CreateInstance();
	static void InitPhysicalDevice();
	static void InitDevice();

	static bool EnumerateSupportedExtension(std::vector<const char* >& extensions, std::vector<const char* >& layers);

	static void DebugMessenger();
};

