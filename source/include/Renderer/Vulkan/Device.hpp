#pragma once
#include <Core/Core.h>
#include <vulkan/vulkan.hpp>

struct QueueFamilyIndices;

namespace Device
{
    void CreateLogicalDevice(VkDevice& _device, VkPhysicalDevice& _physicalDevice, const std::vector<const char*> _validationLayers, VkQueue& _graphicsQueue);
  
}