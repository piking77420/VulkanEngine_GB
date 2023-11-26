#pragma once 
#include <Core/Core.h>
#include "VulkanHeader.hpp"

namespace VkInit
{
	void CreateSurface(VkInstance* _instance, GLFWwindow* _window, VkSurfaceKHR* surface);
	
}