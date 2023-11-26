#pragma once
#include "VulkanConfig.hpp"


struct QueueFamilyIndices;

namespace Device
{
    void CreateLogicalDevice(VkDevice& _device, VkPhysicalDevice& _physicalDevice, const std::vector<const char*> _validationLayers, VkQueue& _graphicsQueue, VkSurfaceKHR& _surface, const std::vector<const char*> _deviceExtensions);
  
}